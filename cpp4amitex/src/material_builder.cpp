#include "amitex/input/material_builder.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "amitex/errors.hpp"
#include "amitex/input/grid.hpp"
#include "amitex/io.hpp"

namespace amitex {

static Vector3D getTangent(Vector3D normal);

void MaterialBuilder::addVoxel(Materials& materials, GridLinPoint pos, const VoxelSpec& spec,
                               Vector3D normal) {
  using std::get;
  using Phase = std::tuple<size_t, double, size_t>;
  // Order phases to avoid duplicating composites
  std::vector<Phase> phase = spec.phases;
  std::sort(phase.begin(), phase.end(), [](Phase a, Phase b) { return get<0>(a) < get<0>(b); });
  if (phase.size() > 1) {
    // Test if order has changed. TODO: strengthen, but needs more normals for nphase > 2 ?
    if (get<0>(phase[0]) != get<0>(spec.phases[0]))
      for (auto& x : normal) x = -x;
  }
  std::vector<size_t> phaseIds, zones;
  std::vector<double> volFracs;
  // Filter rarified phases (AMITEX requirement)
  for (const auto& p : phase) {
    double vf = get<1>(p);
    // Tolerate phases with arbitrary volfracs when it's the only one
    if (phase.size() == 1 || vf > minVolFrac) {
      phaseIds.push_back(get<0>(p));
      volFracs.push_back(vf);
      zones.push_back(get<2>(p));
    }
  }
  // "Empty" voxel, ignore
  if (phaseIds.size() == 0) return;
  for (auto pid : phaseIds) {
    if (pid >= materials.numberMaterials()) {
      materials.setNumberMaterials(pid + 1);
    }
  }
  addVoxelAux(materials, pos, phaseIds, volFracs, zones, normal);
}

void MaterialBuilder::addVoxelAux(Materials& materials, GridLinPoint lpos,
                                  const std::vector<size_t>& phaseIds,
                                  const std::vector<double>& volfracs,
                                  const std::vector<size_t>& zoneIds, Vector3D normal) {
  for (size_t i = 0; i < phaseIds.size(); i++) {
    auto pid = phaseIds[i];
    auto zoneId = zoneIds[i];
    std::vector<Ptr<Zone>>& zones = materials.material(pid)->zones();
    if (zoneId >= zones.size()) {
      for (size_t iz = zones.size(); iz <= zoneId; iz++) {
        zones.push_back(Zone::create({0, 0, 0}));
      }
    }
    zones.at(zoneId)->add(lpos);
  }
  MaterialComposite& composites = materials.composites;
  if (phaseIds.size() > 1) {
    auto it = compos.find(phaseIds);
    if (it == compos.end()) {
      auto [it2, ok] = compos.insert({phaseIds, composites.numberMaterials()});
      if (!ok) throw InputError{std::string{__func__} + " map could not insert"};
      composites.add(Composite::create(phaseIds));
      it = it2;
    }
    size_t ic = it->second;
    std::vector<InterfaceGeometry> geoms;
    for (size_t i = 0; i < phaseIds.size(); i++) {
      for (size_t j = 0; j < phaseIds.size(); j++) {
        geoms.push_back(
            InterfaceGeometry{.normal = normal, .tangent = getTangent(normal), .surface = 1.0});
      }
    }
    composites.at(ic)->addVoxel(lpos, volfracs, geoms, zoneIds);
  }
}

template <typename TVoxel, typename FnAddVoxel>
void buildMaterialsImpl(Materials& materials, GridSize dims, const std::vector<TVoxel>& phases,
                        IndexOrdering ordering, FnAddVoxel fnAddVoxel) {
  MaterialBuilder bd;
  GridSize dimo;
  if (ordering == IndexOrdering::C) {
    dimo = {dims[0], dims[1], dims[2]};
  } else {
    dimo = {dims[2], dims[1], dims[0]};
  }
  Grid grid{dims, {1, 1, 1}};
  size_t index = 0;
  for (size_t i = 0; i < dimo[0]; i++) {
    for (size_t j = 0; j < dimo[1]; j++) {
      for (size_t k = 0; k < dimo[2]; k++) {
        GridLinPoint lpos;
        if (ordering == IndexOrdering::C) {
          lpos = grid.linearize({i, j, k});
        } else {
          lpos = grid.linearize({k, j, i});
        }
        fnAddVoxel(bd, materials, lpos, phases[index]);
        index++;
      }
    }
  }
}

void buildMaterials(Materials& materials, GridSize dims, const std::vector<VoxelSpec>& phases,
                    IndexOrdering ordering) {
  buildMaterialsImpl(materials, dims, phases, ordering,
                     [](MaterialBuilder& bd, Materials& materials, GridLinPoint lpos,
                        const VoxelSpec& phase) { bd.addVoxel(materials, lpos, phase); });
}

void buildMaterials(Materials& materials, GridSize dims,
                    const std::vector<std::tuple<VoxelSpec, Vector3D>>& phases,
                    IndexOrdering ordering) {
  buildMaterialsImpl(materials, dims, phases, ordering,
                     [](MaterialBuilder& bd, Materials& materials, GridLinPoint lpos,
                        const std::tuple<VoxelSpec, Vector3D>& phase) {
                       bd.addVoxel(materials, lpos, std::get<0>(phase), std::get<1>(phase));
                     });
}

void buildMaterials(Materials& materials, GridSize dims, const std::vector<ShortSpec>& phases,
                    IndexOrdering ordering) {
  buildMaterialsImpl(
      materials, dims, phases, ordering,
      [](MaterialBuilder& bd, Materials& materials, GridLinPoint lpos, const ShortSpec& phase) {
        VoxelSpec spec;
        for (auto phis : phase) {
          spec.phases.push_back({static_cast<size_t>(std::get<0>(phis)), std::get<1>(phis), 0});
        }
        bd.addVoxel(materials, lpos, spec);
      });
}

void buildMaterials(Materials& materials, GridSize dims,
                    const std::vector<std::tuple<ShortSpec, Vector3D>>& phases,
                    IndexOrdering ordering) {
  buildMaterialsImpl(
      materials, dims, phases, ordering,
      [](MaterialBuilder& bd, Materials& materials, GridLinPoint lpos,
         const std::tuple<ShortSpec, Vector3D>& phase) {
        VoxelSpec spec;
        for (auto phis : std::get<0>(phase)) {
          spec.phases.push_back({static_cast<size_t>(std::get<0>(phis)), std::get<1>(phis), 0});
        }
        bd.addVoxel(materials, lpos, spec, std::get<1>(phase));
      });
}

void buildMaterials(Materials& materials, GridSize dims, const std::vector<PhaseType>& phases,
                    IndexOrdering ordering) {
  buildMaterialsImpl(
      materials, dims, phases, ordering,
      [](MaterialBuilder& bd, Materials& materials, GridLinPoint lpos, const PhaseType& phase) {
        VoxelSpec spec;
        spec.phases.push_back({phase, 1.0, 0});
        bd.addVoxel(materials, lpos, spec);
      });
}

Grid buildMaterialsFromVtk(Materials& materials, const std::string& materialIdPath,
                           const std::string& zoneIdPath, int minId) {
  if (materialIdPath.size() == 0 && zoneIdPath.size() == 0)
    throw InputError{"One of materialIdPath or zoneIdPath  arguments must be specified"};
  VtkHeader header;
  std::vector<int64_t> matIds, zoneIds;
  if (materialIdPath.size() > 0) {
    try {
      header = readVTK(materialIdPath, matIds);
    } catch (std::runtime_error& e) {
      throw InputError{materialIdPath + ": " + e.what()};
    }
  }
  if (zoneIdPath.size() > 0) {
    try {
      header = readVTK(zoneIdPath, zoneIds);
    } catch (std::runtime_error& e) {
      throw InputError{zoneIdPath + ": " + e.what()};
    }
  }
  if (materialIdPath.size() == 0) {
    matIds.resize(zoneIds.size());
    for (auto& x : matIds) x = minId;
  }
  if (zoneIdPath.size() == 0) {
    zoneIds.resize(matIds.size());
    for (auto& x : zoneIds) x = minId;
  }
  if (zoneIds.size() != matIds.size()) {
    throw InputError{
        std::string{"Inconsistent dimensions between " + materialIdPath + " and " + zoneIdPath}};
  }
  MaterialBuilder bd;
  size_t counter = 0;
  for (size_t lpos = 0; lpos < matIds.size(); lpos++) {
    if (matIds[lpos] < minId)
      throw InputError{"Material ID " + std::to_string(matIds[lpos]) + " <  min Id (" +
                       std::to_string(minId) + ")"};
    if (zoneIds[lpos] < minId)
      throw InputError{"Zone ID " + std::to_string(zoneIds[lpos]) + " <  min Id (" +
                       std::to_string(minId) + ")"};
    bd.addVoxel(materials, lpos, matIds[lpos] - minId, zoneIds[lpos] - minId);
  }
  Grid grid{header.dimensions, header.spacing};
  grid.setOrigin(header.origin);
  return grid;
}

static Vector3D getTangent(Vector3D normal) {
  Vector3D ret;
  if (normal[0] == 0.0 && normal[1] == 0.0 && normal[2] == 0.0) return normal;
  size_t imax = 0;
  for (size_t i = 1; i < normal.size(); i++) {
    if (std::abs(normal[i]) > std::abs(normal[imax])) imax = i;
  }
  // Arbitrarily put t coords at 1 except for imax: sum(-normal[i!=imax])/normal[imax]
  double num = 0.0;
  for (size_t i = 0; i < ret.size(); i++) {
    if (i != imax) {
      ret[i] = 1.0;
      num += normal[i];
    }
  }
  ret[imax] = -num / normal[imax];
  double norm2 = 0.0;
  for (size_t i = 0; i < ret.size(); i++) norm2 += ret[i] * ret[i];
  for (size_t i = 0; i < ret.size(); i++) ret[i] /= std::sqrt(norm2);

#ifdef AMITEX_DEBUG
  double prod2 = 0.0;
  for (size_t i = 0; i < ret.size(); i++) prod2 += ret[i] * normal[i];
  assert(prod2 == prod2 && std::abs(prod2) < 1.e-7);
#endif

  return ret;
}

}  // namespace amitex
