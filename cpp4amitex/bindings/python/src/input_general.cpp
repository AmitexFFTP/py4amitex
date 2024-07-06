#include "input_general.hpp"

#include <sstream>
#include <variant>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "amitex/input.hpp"

namespace py = pybind11;
using namespace amitex;
using namespace pybind11::literals;

// PYBIND11_MAKE_OPAQUE(std::vector<GridLinPoint>);

// <ELEMENT Value="value"> variety
#define DEF_VALUE_PROPERTY(name, TYPE, OBJTYPE)                      \
  def_property(                                                      \
      #name, [](OBJTYPE& a) -> Value<TYPE>& { return a.name; },      \
      [](OBJTYPE& a, const std::variant<TYPE, Value<TYPE>>& value) { \
        if (std::holds_alternative<TYPE>(value)) {                   \
          a.name.value = std::get<TYPE>(value);                      \
        } else {                                                     \
          a.name = std::get<Value<TYPE>>(value);                     \
        }                                                            \
      })

// <ELEMENT Type="type"> variety
#define DEF_TYPE_PROPERTY(name, OBJTYPE)                 \
  def_property(                                          \
      #name, [](OBJTYPE& a) -> Type& { return a.name; }, \
      [](OBJTYPE& a, const std::string& value) { a.name.type = value; })

template <typename T>
static std::string toXML(const T& obj) {
  std::ostringstream ret;
  writeXML(ret, obj);
  return ret.str();
}

namespace amitex_python {

void defineInputMod(py::module_& m) {
  m.doc() = R"pbdoc(
        AMITEX Input generator 
        -----------------------

        .. currentmodule:: amitex.input

        .. autosummary::
           :toctree: _generate

           Algorithm
    )pbdoc";

  // py::bind_vector<std::vector<GridLinPoint>>(m, "VectorGridLinePoint");
  py::class_<Value<bool>>(m, "ValueBool")
      .def(py::init<const char*>())
      .def_readwrite("value", &Value<bool>::value);
  py::class_<Value<int>>(m, "ValueInt")
      .def(py::init<const char*>())
      .def_readwrite("value", &Value<int>::value);
  py::class_<Value<double>>(m, "ValueDouble")
      .def(py::init<const char*>())
      .def_readwrite("value", &Value<double>::value);
  py::class_<Value<std::string>>(m, "ValueString")
      .def(py::init<const char*>())
      .def_readwrite("value", &Value<std::string>::value);
  py::class_<Type>(m, "Type").def(py::init<const char*>()).def_readwrite("type", &Type::type);

  py::class_<ConvergenceAcceleration>(m, "ConvergenceAcceleration")
      .def(py::init<>())
      .def(py::init<bool, std::optional<int>>(), "value"_a, "modACV"_a = std::nullopt)
      .def_readwrite("value", &ConvergenceAcceleration::value)
      .def_readwrite("modACV", &ConvergenceAcceleration::modACV);

  py::class_<ConvergenceForced>(m, "ConvergenceForced")
      .def(py::init<>())
      .def(py::init<bool, int, int>(), "value"_a, "nitCVFor"_a = 0, "nCVFor"_a = 0)
      .def_readwrite("value", &ConvergenceForced::value)
      .def_readwrite("nitCVFor", &ConvergenceForced::nitCVFor)
      .def_readwrite("nCVFor", &ConvergenceForced::nCVFor);

  py::class_<Substepping>(m, "Substepping")
      .def(py::init<>())
      .def_readwrite("nitermax2", &Substepping::nitermax2)
      .def_readwrite("geomRatio", &Substepping::geomRatio)
      .def_readwrite("nsub", &Substepping::nsub)
      .def_readwrite("depth", &Substepping::depth);

  py::class_<Algorithm>(m, "Algorithm")
      .def(py::init<>())
      .def(py::init<const std::string&, const std::variant<bool, ConvergenceAcceleration>&,
                    std::optional<double>, std::optional<int>, std::optional<double>,
                    std::optional<double>, std::optional<int>, std::optional<int>,
                    const std::optional<std::string>&, const std::optional<Substepping>&,
                    const std::optional<ConvergenceForced>&>(),
           "type"_a, "convergenceAcceleration"_a, "convergenceCriterion"_a = std::nullopt,
           "nitermax"_a = std::nullopt, "convergenceCriterionSmacro"_a = std::nullopt,
           "convergenceCriterionCompatibility"_a = std::nullopt, "nitermin"_a = std::nullopt,
           "niterminACV"_a = std::nullopt, "initialize"_a = std::nullopt,
           "substepping"_a = std::nullopt, "convergenceForced"_a = std::nullopt)
      .def_readwrite("type", &Algorithm::type)
      .def_property(
          "convergenceAcceleration",
          [](Algorithm& a) -> ConvergenceAcceleration& { return a.convergenceAcceleration; },
          [](Algorithm& a, const std::variant<bool, ConvergenceAcceleration>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.convergenceAcceleration = std::get<0>(cv);
            } else {
              a.convergenceAcceleration = std::get<1>(cv);
            }
          })
      .def_property(
          "convergenceForced",
          [](Algorithm& a) -> std::optional<ConvergenceForced>& { return a.convergenceForced; },
          [](Algorithm& a, const std::variant<ConvergenceForced, bool>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.convergenceForced = std::get<bool>(cv);
            } else {
              a.convergenceForced = std::get<ConvergenceForced>(cv);
            }
          })
      .DEF_VALUE_PROPERTY(convergenceCriterion, double, Algorithm)
      .DEF_VALUE_PROPERTY(nitermax, int, Algorithm)
      .DEF_VALUE_PROPERTY(convergenceCriterionCompatibility, double, Algorithm)
      .DEF_VALUE_PROPERTY(initialize, std::string, Algorithm)
      .DEF_VALUE_PROPERTY(convergenceCriterionSmacro, double, Algorithm)
      .DEF_VALUE_PROPERTY(nitermin, int, Algorithm)
      .DEF_VALUE_PROPERTY(niterminACV, int, Algorithm)
      .def_readwrite("substepping", &Algorithm::substepping);

  py::class_<SmallPerturbations>(m, "SmallPerturbations")
      .def(py::init<>())
      .def(py::init<bool, const std::string&>(), "value"_a, "displacementGradient"_a = "")
      .def_readwrite("value", &SmallPerturbations::value)
      .def_readwrite("displacementGradient", &SmallPerturbations::displacementGradient);

  py::class_<Mechanics>(m, "Mechanics")
      .def(py::init<>())
      .def(py::init<const std::string&, std::variant<SmallPerturbations, bool>>(), "filter"_a,
           "smallPerturbations"_a)
      .def_property(
          "smallPerturbations",
          [](const Mechanics& a) -> const SmallPerturbations& { return a.smallPerturbations; },
          [](Mechanics& a, const std::variant<SmallPerturbations, bool>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.smallPerturbations = std::get<bool>(cv);
            } else {
              a.smallPerturbations = std::get<SmallPerturbations>(cv);
            }
          })
      .DEF_TYPE_PROPERTY(filter, Mechanics);

  py::class_<Diffusion>(m, "Diffusion")
      .def(py::init<>())
      .def(py::init<const std::string&, bool>(), "filter"_a, "stationary"_a)
      .DEF_VALUE_PROPERTY(stationary, bool, Diffusion)
      .DEF_TYPE_PROPERTY(filter, Diffusion);

  py::class_<AlgorithmLaminate>(m, "AlgorithmLaminate")
      .def(py::init<>())
      .def_property(
          "convergenceAcceleration",
          [](AlgorithmLaminate& a) -> ConvergenceAcceleration& {
            return a.convergenceAcceleration;
          },
          [](AlgorithmLaminate& a, const std::variant<ConvergenceAcceleration, bool>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.convergenceAcceleration = std::get<bool>(cv);
            } else {
              a.convergenceAcceleration = std::get<ConvergenceAcceleration>(cv);
            }
          })
      .DEF_VALUE_PROPERTY(convergenceCriterion, double, AlgorithmLaminate)
      .DEF_VALUE_PROPERTY(nIncrements, int, AlgorithmLaminate)
      .DEF_VALUE_PROPERTY(initializationType, std::string, AlgorithmLaminate);

  py::class_<AlgorithmParameters>(m, "AlgorithmParameters")
      .def(py::init())
      .def(py::init<const Algorithm&, const std::optional<Mechanics>&,
                    const std::optional<Diffusion>&>(),
           "algorithm"_a, "mechanics"_a = std::nullopt, "diffusion"_a = std::nullopt)
      .def_readwrite("algorithm", &AlgorithmParameters::algorithm)
      .def_readwrite("mechanics", &AlgorithmParameters::mechanics)
      .def_readwrite("diffusion", &AlgorithmParameters::diffusion)
      .def_readwrite("algorithmLaminate", &AlgorithmParameters::algorithmLaminate)
      .def("toXML", &toXML<AlgorithmParameters>);

  py::enum_<MechanicDriving>(m, "MechanicDriving")
      .value("Stress", MechanicDriving::Stress)
      .value("Strain", MechanicDriving::Strain);

  py::enum_<DiffusionDriving>(m, "DiffusionDriving")
      .value("Flux", DiffusionDriving::Flux)
      .value("Gradient", DiffusionDriving::Gradient);

  py::enum_<Evolution>(m, "Evolution")
      .value("Constant", Evolution::Constant)
      .value("Linear", Evolution::Linear);

  struct component_t {};
  auto comp = py::class_<component_t>(m, "Component");
  comp.attr("XX") = Component::XX;
  comp.attr("YY") = Component::YY;
  comp.attr("ZZ") = Component::ZZ;
  comp.attr("XY") = Component::XY;
  comp.attr("XZ") = Component::XZ;
  comp.attr("YZ") = Component::YZ;
  comp.attr("YX") = Component::YX;
  comp.attr("ZX") = Component::ZX;
  comp.attr("ZY") = Component::ZY;
  comp.attr("X") = Component::X;
  comp.attr("Y") = Component::Y;
  comp.attr("Z") = Component::Z;

  py::enum_<DirStress>(m, "DirStress")
      .value("Cauchy", DirStress::Cauchy)
      .value("PK1", DirStress::PK1);

  py::class_<Loading>(m, "Loading")
      .def(py::init<>())
      .def("setTimeDiscretizationUser", static_cast<void (Loading::*)(const std::vector<double>&)>(
                                            &Loading::setTimeDiscretizationUser))
      .def("setTimeDiscretizationLinear", &Loading::setTimeDiscretizationLinear)
      .def("setEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving, Evolution, double)>(
               &Loading::setEvolution),
           "componant"_a, "driving"_a, "evolution"_a, "value"_a = 0.0)
      .def("setEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving, Evolution, double,
                                         std::optional<double>)>(&Loading::setEvolution),
           "component"_a, "driving"_a, "evolution"_a, "value"_a = 0.0, "dirstress"_a = std::nullopt)
      .def("setConstantEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving,
                                         std::optional<double> dirstress)>(
               &Loading::setConstantEvolution),
           "component"_a, "driving"_a, "dirstress"_a = std::nullopt)
      .def("setConstantEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving)>(&Loading::setConstantEvolution))
      .def("setLinearEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving, double,
                                         std::optional<double> dirstress)>(
               &Loading::setLinearEvolution),
           "component"_a, "driving"_a, "value"_a = 0.0, "dirstress"_a = std::nullopt)
      .def("setLinearEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving, double)>(
               &Loading::setLinearEvolution),
           "component"_a, "driving"_a, "value"_a = 0.0)
      .def("setDirStress", &Loading::setDirStress)
      .def_property("dirStress", nullptr, &Loading::setDirStress)
      .def("setOutputVtkList", &Loading::setOutputVtkList)
      .def_property("outputVtkList", nullptr, &Loading::setOutputVtkList)
      .def("setOutputCell", &Loading::setOutputCell)
      .def_property("outputCell", nullptr, &Loading::setOutputCell)
      .def("setOutputZone", &Loading::setOutputZone)
      .def_property("outputZone", nullptr, &Loading::setOutputZone)
      .def("setGradGradU", &Loading::setGradGradU, "ij"_a, "k"_a, "evolution"_a, "value"_a = 0.0)
      .def_property("userInterruptValues", nullptr, &Loading::setUserInterruptValues)
      .def("setTemperatureEvolution", &Loading::setTemperatureEvolution, "evolution"_a,
           "value"_a = 0.0)
      .def("setParamEvolution", &Loading::setParamEvolution, "index"_a, "evolution"_a,
           "value"_a = 0.0)
      .def("addUserInterruptValue", &Loading::addUserInterruptValue)
      .def("setUserInterruptValues", &Loading::setUserInterruptValues);

  py::class_<VtkStressStrain>(m, "VtkStressStrain")
      .def(py::init<int, int>(), "stress"_a = 0, "strain"_a = 0);

  py::class_<VtkFluxDGradD>(m, "VtkFluxDGradD")
      .def(py::init<int, int>(), "fluxd"_a = 0, "gradd"_a = 0);

  py::class_<Output>(m, "Output")
      .def(py::init<>())
      .def("setVtkStressStrain", &Output::setVtkStressStrain, "stress"_a, "strain"_a)
      .def_readwrite("vtkStressStrain", &Output::vtkStressStrain)
      .def("setVtkFluxDGradD", &Output::setVtkFluxDGradD, "fluxD"_a, "gradD"_a)
      .def_readwrite("vtkFluxDGradD", &Output::vtkFluxDGradD)
      .def("addZone",
           static_cast<void (Output::*)(size_t, const std::vector<size_t>&)>(&Output::addZone),
           "numM"_a, "intVarList"_a = std::vector<size_t>{})
      .def("addVtkIntVarList", &Output::addVtkIntVarList);

  py::class_<InitLoadExt>(m, "InitLoadExt")
      .def(py::init())
      .DEF_VALUE_PROPERTY(temperature, double, InitLoadExt)
      .def("setParam", &InitLoadExt::setParam);

  py::class_<LoadingOutput>(m, "LoadingOutput")
      .def(py::init<>())
      .def("add", static_cast<void (LoadingOutput::*)(const Loading&)>(&LoadingOutput::add))
      .def_readwrite("output", &LoadingOutput::output)
      .def_readwrite("initLoadExt", &LoadingOutput::initLoadExt)
      .def("toXML", &toXML<LoadingOutput>);

  py::class_<Zone>(m, "Zone")
      .def(py::init<GridSize, const std::vector<GridPoint>&>(), "gridSize"_a,
           "positions"_a = std::vector<GridPoint>{})
      .def("add", static_cast<void (Zone::*)(GridPoint)>(&Zone::add))
      .def("add", static_cast<void (Zone::*)(GridLinPoint)>(&Zone::add))
      .def("add",
           [](Zone& zone, py::iterator it) {
             while (it != py::iterator::sentinel()) {
               GridPoint p = it->cast<GridPoint>();
               zone.add(p);
               ++it;
             }
           })
      .def("numberVoxels", &Zone::numberVoxels)
      .def("linearPositions", &Zone::linearPositions);

  py::class_<Material>(m, "Material")
      .def(py::init<>())
      .def("setLaw", &Material::setLaw, "law"_a, "lib"_a = "")
      .def("setLawK", &Material::setLawK, "law"_a, "lib"_a = "")
      .def_property("law", nullptr, [](Material& m, const std::string& s) { m.setLaw(s); })
      .def_property("lawK", nullptr, [](Material& m, const std::string& s) { m.setLawK(s); })
      .def("setCoeff", &Material::setCoeff)
      .def("setCoeffZone", &Material::setCoeffZone)
      .def("setCoeffZoneFromBin", &Material::setCoeffZoneFromBin)
      .def("setCoeffs", &Material::setCoeffs)
      .def("setCoeffK", &Material::setCoeffK)
      .def("setCoeffKZone", &Material::setCoeffKZone)
      .def("setCoeffKZoneFromBin", &Material::setCoeffKZoneFromBin)
      .def("setCoeffKs", &Material::setCoeffKs)
      .def("setCoeffComposite", &Material::setCoeffComposite)
      .def("setCoeffCompositeZone", &Material::setCoeffCompositeZone)
      .def("setCoeffCompositeZoneFromBin", &Material::setCoeffCompositeZoneFromBin)
      .def("setCoeffComposites", &Material::setCoeffComposites)
      .def_property("coeffs", nullptr, &Material::setCoeffs)
      .def_property("coeffKs", nullptr, &Material::setCoeffKs)
      .def_property("coeffComposites", nullptr, &Material::setCoeffComposites)
      .def("setNumberCoeff", &Material::setNumberCoeff)
      .def("setNumberCoeffK", &Material::setNumberCoeffK)
      .def("setNumberCoeffComposite", &Material::setNumberCoeffComposite)
      .def("numberCoeff", &Material::numberCoeff)
      .def("numberCoeffK", &Material::numberCoeffK)
      .def("numberCoeffComposite", &Material::numberCoeffComposite)
      .def("addZone", &Material::addZone, "zone"_a, "coeffs"_a = std::vector<double>{},
           "coeffKs"_a = std::vector<double>{}, "coeffComposites"_a = std::vector<double>{})
      .def("numberZones", &Material::numberZones)
      .def("numberIntVars", &Material::numberIntVars)
      .def("addIntVar", static_cast<void (Material::*)(double)>(&Material::addIntVar))
      .def("addIntVar",
           [](Material& mat, const Field<double>& f) { mat.addIntVar(f.shallowCopy()); })
      .def("setCoeffName", &Material::setCoeffName)
      .def("setCoeffKName", &Material::setCoeffKName)
      .def("setCoeffCompositeName", &Material::setCoeffCompositeName)
      .def("setIntVarName", &Material::setIntVarName);

  py::class_<InterfaceGeometry>(m, "InterfaceGeometry")
      .def(py::init())
      .def_readwrite("normal", &InterfaceGeometry::normal)
      .def_readwrite("tangent", &InterfaceGeometry::tangent)
      .def_readwrite("surface", &InterfaceGeometry::surface);

  py::class_<Composite>(m, "Composite")
      .def(py::init<const std::vector<size_t>&, const std::string&>(), "phases"_a, "law"_a = "")
      .def("addVoxel",
           static_cast<void (Composite::*)(GridLinPoint, const std::vector<double>&,
                                           const std::vector<InterfaceGeometry>&)>(
               &Composite::addVoxel),
           "position"_a, "phi"_a, "geom"_a = std::vector<InterfaceGeometry>{})
      .def("setLaw", static_cast<void (Composite::*)(const std::string&)>(&Composite::setLaw))
      .def_property("law", nullptr, &Composite::setLaw)
      .def("numberPhases", &Composite::numberPhases)
      .def("materialIndices", &Composite::materialIndices);

  py::class_<ReferenceMaterial>(m, "ReferenceMaterial")
      .def(py::init<double, double>(), "lambda0"_a, "mu0"_a);

  py::class_<ReferenceMaterialD>(m, "ReferenceMaterialD").def(py::init<double>(), "K0"_a);

  py::class_<Materials>(m, "Materials")
      .def(py::init<>())
      .def("add", static_cast<void (Materials::*)(const Material&)>(&Materials::add))
      .def("add", static_cast<void (Materials::*)(const Composite&)>(&Materials::add))
      .def_readwrite("referenceMaterial", &Materials::referenceMaterial)
      .def_readwrite("referenceMaterialD", &Materials::referenceMaterialD)
      .def("numberMaterials", &Materials::numberMaterials)
      .def("numberComposites", &Materials::numberComposites)
      .def("toXML", &toXML<Materials>)
      // .def("material", [](Materials& m, size_t i) -> Material& { return m.material(i); })
      .def("material", static_cast<Material& (Materials::*)(size_t)>(&Materials::material),
           py::return_value_policy::reference)
      .def("composite", static_cast<Composite& (Materials::*)(int)>(&Materials::composite),
           py::return_value_policy::reference);

  py::class_<Grid>(m, "Grid")
      .def(py::init<GridSize, Vector3D>(), "dims"_a, "dx"_a)
      .def("setPbc", &Grid::setPbc)
      .def_property("pbc", nullptr, &Grid::setPbc)
      .def("setOrigin", &Grid::setOrigin)
      .def_property("origin", nullptr, &Grid::setOrigin)
      .def("totalSize", &Grid::totalSize)
      .def("dims", &Grid::dims)
      .def("voxelLengths", &Grid::voxelLengths)
      .def("linearize", &Grid::linearize)
      .def("distance", static_cast<double (Grid::*)(const GridPoint& a, const GridPoint& b) const>(
                           &Grid::distance))
      .def("distance", static_cast<double (Grid::*)(const GridPoint& a, const Vector3D& b) const>(
                           &Grid::distance))
      .def("allPoints",
           [](Grid& grid) {
             auto it = grid.allPoints();
             return py::make_iterator(it.begin(), it.end());
           })
      .def("allLinPoints", [](Grid& grid) {
        auto it = grid.allLinPoints();
        return py::make_iterator(it.begin(), it.end());
      });

  py::class_<Input>(m, "Input")
      .def(py::init<>())
      .def(py::init([](Grid& grid, AlgorithmParameters& algorithmParameters, Materials& materials,
                       LoadingOutput& loadingOutput) {
             return Input{grid, std::move(algorithmParameters), std::move(materials),
                          std::move(loadingOutput)};
           }),
           "note: AlgorithmParameters, Materials, LoadingOutput passed to constructor are moved "
           "for performance reasons, invalidating their content",
           "grid"_a, "algorithmParameters"_a, "materials"_a, "loadingOutput"_a)
      .def_readwrite("grid", &Input::grid)
      .def_readwrite("loadingOutput", &Input::loadingOutput)
      .def_readwrite("algorithmParameters", &Input::algorithmParameters)
      .def_readwrite("materials", &Input::materials)
      .def_readwrite("resultsDir", &Input::resultsDir)
      .def("outputPrefix", &Input::outputPrefix)
      .def("generateFiles", &Input::generateFiles);

  m.attr("__version__") = "dev";
}

}  // namespace amitex_python
