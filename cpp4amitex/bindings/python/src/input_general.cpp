#include "input_general.hpp"

#include <sstream>
#include <variant>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include "amitex/config.hpp"
#include "amitex/input.hpp"

#include "amitex/input/common.hpp"
#include "docstrings.hpp"

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
      },                                                             \
      DOC(amitex, OBJTYPE, name))

// <ELEMENT Type="type"> variety
#define DEF_TYPE_PROPERTY(name, OBJTYPE)                                 \
  def_property(                                                          \
      #name, [](OBJTYPE& a) -> Type& { return a.name; },                 \
      [](OBJTYPE& a, const std::string& value) { a.name.type = value; }, \
      DOC(amitex, OBJTYPE, name))

template <typename T>
static std::string toXML(const T& obj) {
  std::ostringstream ret;
  writeXML(ret, obj);
  return ret.str();
}

namespace amitex_python {

void defineInputMod(py::module_& m) {
  m.doc() = R"pbdoc(Input
=====


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

  py::class_<ConvergenceAcceleration>(m, "ConvergenceAcceleration",
                                      DOC(amitex, ConvergenceAcceleration))
      .def(py::init<>())
      .def(py::init<bool, std::optional<int>>(), "value"_a, "modACV"_a = std::nullopt,
           DOC(amitex, ConvergenceAcceleration, ConvergenceAcceleration))
      .def_readwrite("value", &ConvergenceAcceleration::value,
                     DOC(amitex, ConvergenceAcceleration, value))
      .def_readwrite("modACV", &ConvergenceAcceleration::modACV,
                     DOC(amitex, ConvergenceAcceleration, modACV));

  py::class_<ConvergenceForced>(m, "ConvergenceForced", DOC(amitex, ConvergenceForced))
      .def(py::init<>())
      .def(py::init<bool, int, int>(), "value"_a, "nitCVFor"_a = 0, "nCVFor"_a = 0,
           DOC(amitex, ConvergenceForced, ConvergenceForced, 2))
      .def_readwrite("value", &ConvergenceForced::value, DOC(amitex, ConvergenceForced, value))
      .def_readwrite("nitCVFor", &ConvergenceForced::nitCVFor,
                     DOC(amitex, ConvergenceForced, nitCVFor))
      .def_readwrite("nCVFor", &ConvergenceForced::nCVFor, DOC(amitex, ConvergenceForced, nCVFor));

  py::class_<Substepping, Ptr<Substepping>>(m, "Substepping", DOC(amitex, Substepping))
      .def(py::init(&Substepping::create))
      .def_readwrite("nitermax2", &Substepping::nitermax2, DOC(amitex, Substepping, nitermax2))
      .def_readwrite("geomRatio", &Substepping::geomRatio, DOC(amitex, Substepping, geomRatio))
      .def_readwrite("nsub", &Substepping::nsub, DOC(amitex, Substepping, nsub))
      .def_readwrite("depth", &Substepping::depth, DOC(amitex, Substepping, depth));

  py::class_<Algorithm, Ptr<Algorithm>>(m, "Algorithm", DOC(amitex, Algorithm))
      .def(py::init(&Algorithm::create), "type"_a, "convergenceAcceleration"_a,
           "convergenceCriterion"_a = std::nullopt, "nitermax"_a = std::nullopt,
           "convergenceCriterionSmacro"_a = std::nullopt,
           "convergenceCriterionCompatibility"_a = std::nullopt, "nitermin"_a = std::nullopt,
           "niterminACV"_a = std::nullopt, "initialize"_a = std::nullopt, "substepping"_a = nullptr,
           "convergenceForced"_a = std::nullopt, DOC(amitex, Algorithm, Algorithm, 2))
      .def_static("createDefault", &Algorithm::createDefault)
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
          },
          DOC(amitex, Algorithm, convergenceAcceleration))
      .def_property(
          "convergenceForced",
          [](Algorithm& a) -> std::optional<ConvergenceForced>& { return a.convergenceForced; },
          [](Algorithm& a, const std::variant<ConvergenceForced, bool>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.convergenceForced = std::get<bool>(cv);
            } else {
              a.convergenceForced = std::get<ConvergenceForced>(cv);
            }
          },
          DOC(amitex, Algorithm, convergenceForced))
      .DEF_VALUE_PROPERTY(convergenceCriterion, double, Algorithm)
      .DEF_VALUE_PROPERTY(nitermax, int, Algorithm)
      .DEF_VALUE_PROPERTY(convergenceCriterionCompatibility, double, Algorithm)
      .DEF_VALUE_PROPERTY(initialize, std::string, Algorithm)
      .DEF_VALUE_PROPERTY(convergenceCriterionSmacro, double, Algorithm)
      .DEF_VALUE_PROPERTY(nitermin, int, Algorithm)
      .DEF_VALUE_PROPERTY(niterminACV, int, Algorithm)
      .def_readwrite("substepping", &Algorithm::substepping, DOC(amitex, Algorithm, substepping));

  py::class_<SmallPerturbations>(m, "SmallPerturbations", DOC(amitex, SmallPerturbations))
      .def(py::init<>())
      .def(py::init<bool, const std::string&>(), "value"_a, "displacementGradient"_a = "",
           DOC(amitex, SmallPerturbations, SmallPerturbations, 2))
      .def_readwrite("value", &SmallPerturbations::value, DOC(amitex, SmallPerturbations, value))
      .def_readwrite("displacementGradient", &SmallPerturbations::displacementGradient,
                     DOC(amitex, SmallPerturbations, displacementGradient));

  py::class_<Mechanics, Ptr<Mechanics>>(m, "Mechanics", DOC(amitex, Mechanics))
      .def(py::init(&Mechanics::create), "filter"_a, "smallPerturbations"_a,
           DOC(amitex, Mechanics, smallPerturbations))
      .def_static("createDefault", &Mechanics::createDefault)
      .def_property(
          "smallPerturbations",
          [](const Mechanics& a) -> const SmallPerturbations& { return a.smallPerturbations; },
          [](Mechanics& a, const std::variant<SmallPerturbations, bool>& cv) {
            if (std::holds_alternative<bool>(cv)) {
              a.smallPerturbations = std::get<bool>(cv);
            } else {
              a.smallPerturbations = std::get<SmallPerturbations>(cv);
            }
          },
          DOC(amitex, Mechanics, smallPerturbations))
      .DEF_TYPE_PROPERTY(filter, Mechanics);

  py::class_<Diffusion, Ptr<Diffusion>>(m, "Diffusion", DOC(amitex, Diffusion))
      .def(py::init(&Diffusion::create), "filter"_a, "stationary"_a,
           DOC(amitex, Diffusion, Diffusion, 2))
      .def_static("createDefault", &Diffusion::createDefault)
      .DEF_VALUE_PROPERTY(stationary, bool, Diffusion)
      .DEF_TYPE_PROPERTY(filter, Diffusion);

  py::class_<AlgorithmLaminate, Ptr<AlgorithmLaminate>>(m, "AlgorithmLaminate",
                                                        DOC(amitex, AlgorithmLaminate))
      .def(py::init(&AlgorithmLaminate::create))
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
          },
          DOC(amitex, AlgorithmLaminate, convergenceAcceleration))
      .DEF_VALUE_PROPERTY(convergenceCriterion, double, AlgorithmLaminate)
      .DEF_VALUE_PROPERTY(nIncrements, int, AlgorithmLaminate)
      .DEF_VALUE_PROPERTY(initializationType, std::string, AlgorithmLaminate)
      .DEF_VALUE_PROPERTY(nMaxSubdivision, int, AlgorithmLaminate);

  py::class_<AlgorithmParameters, Ptr<AlgorithmParameters>>(m, "AlgorithmParameters",
                                                            DOC(amitex, AlgorithmParameters))
      .def(py::init(&AlgorithmParameters::create), "algorithm"_a, "mechanics"_a = nullptr,
           "diffusion"_a = nullptr, DOC(amitex, AlgorithmParameters, AlgorithmParameters))
      .def_readwrite("algorithm", &AlgorithmParameters::algorithm,
                     DOC(amitex, AlgorithmParameters, algorithm))
      .def_readwrite("mechanics", &AlgorithmParameters::mechanics,
                     DOC(amitex, AlgorithmParameters, mechanics))
      .def_readwrite("diffusion", &AlgorithmParameters::diffusion,
                     DOC(amitex, AlgorithmParameters, diffusion))
      .def_readwrite("algorithmLaminate", &AlgorithmParameters::algorithmLaminate,
                     DOC(amitex, AlgorithmParameters, algorithmLaminate))
      .def("toXML", &toXML<AlgorithmParameters>);

  py::enum_<MechanicDriving>(m, "MechanicDriving", DOC(amitex, MechanicDriving))
      .value("Stress", MechanicDriving::Stress)
      .value("Strain", MechanicDriving::Strain);

  py::enum_<DiffusionDriving>(m, "DiffusionDriving", DOC(amitex, DiffusionDriving))
      .value("Flux", DiffusionDriving::Flux)
      .value("Gradient", DiffusionDriving::Gradient);

  py::enum_<Evolution>(m, "Evolution", DOC(amitex, Evolution))
      .value("Constant", Evolution::Constant)
      .value("Linear", Evolution::Linear);

  struct component_t {};
  auto comp = py::class_<component_t>(m, "Component", DOC(amitex, Component));
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

  py::enum_<DirStress>(m, "DirStress", DOC(amitex, DirStress))
      .value("Cauchy", DirStress::Cauchy)
      .value("PK1", DirStress::PK1);

  py::class_<Loading, Ptr<Loading>>(m, "Loading", DOC(amitex, Loading))
      .def(py::init(&Loading::create))
      .def("setTimeDiscretizationUser",
           static_cast<void (Loading::*)(const std::vector<double>&)>(
               &Loading::setTimeDiscretizationUser),
           DOC(amitex, Loading, setTimeDiscretizationUser))
      .def("setTimeDiscretizationLinear", &Loading::setTimeDiscretizationLinear,
           DOC(amitex, Loading, setTimeDiscretizationLinear))
      .def("setEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving, Evolution, double)>(
               &Loading::setEvolution),
           "componant"_a, "driving"_a, "evolution"_a, "value"_a = 0.0,
           DOC(amitex, Loading, setEvolution))
      .def("setEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving, Evolution, double,
                                         std::optional<double>)>(&Loading::setEvolution),
           "component"_a, "driving"_a, "evolution"_a, "value"_a = 0.0, "dirstress"_a = std::nullopt,
           DOC(amitex, Loading, setEvolution))
      .def("setConstantEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving,
                                         std::optional<double> dirstress)>(
               &Loading::setConstantEvolution),
           "component"_a, "driving"_a, "dirstress"_a = std::nullopt,
           DOC(amitex, Loading, setConstantEvolution))
      .def("setConstantEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving)>(&Loading::setConstantEvolution),
           DOC(amitex, Loading, setConstantEvolution))
      .def("setLinearEvolution",
           static_cast<void (Loading::*)(std::pair<int, int>, MechanicDriving, double,
                                         std::optional<double> dirstress)>(
               &Loading::setLinearEvolution),
           "component"_a, "driving"_a, "value"_a = 0.0, "dirstress"_a = std::nullopt,
           DOC(amitex, Loading, setLinearEvolution))
      .def("setLinearEvolution",
           static_cast<void (Loading::*)(int, DiffusionDriving, double)>(
               &Loading::setLinearEvolution),
           "component"_a, "driving"_a, "value"_a = 0.0, DOC(amitex, Loading, setLinearEvolution))
      .def("setDirStress", &Loading::setDirStress, DOC(amitex, Loading, setDirStress))
      .def_property("dirStress", nullptr, &Loading::setDirStress)
      .def("setOutputVtkList", &Loading::setOutputVtkList, DOC(amitex, Loading, setOutputVtkList))
      .def_property("outputVtkList", nullptr, &Loading::setOutputVtkList)
      .def("setOutputCell", &Loading::setOutputCell, DOC(amitex, Loading, setOutputCell))
      .def_property("outputCell", nullptr, &Loading::setOutputCell)
      .def("setOutputZone", &Loading::setOutputZone, DOC(amitex, Loading, setOutputZone))
      .def_property("outputZone", nullptr, &Loading::setOutputZone)
      .def("setGradGradU", &Loading::setGradGradU, "ij"_a, "k"_a, "evolution"_a, "value"_a = 0.0,
           DOC(amitex, Loading, setGradGradU))
      .def_property("userInterruptValues", nullptr, &Loading::setUserInterruptValues)
      .def("setTemperatureEvolution", &Loading::setTemperatureEvolution, "evolution"_a,
           "value"_a = 0.0, DOC(amitex, Loading, setTemperatureEvolution))
      .def("setParamEvolution", &Loading::setParamEvolution, "index"_a, "evolution"_a,
           "value"_a = 0.0, DOC(amitex, Loading, setParamEvolution))
      .def("addUserInterruptValue", &Loading::addUserInterruptValue,
           DOC(amitex, Loading, addUserInterruptValue))
      .def("setUserInterruptValues", &Loading::setUserInterruptValues,
           DOC(amitex, Loading, setUserInterruptValues))
      .def("setRestart", &Loading::setRestart, "every"_a, DOC(amitex, Loading, setRestart));

  py::class_<VtkStressStrain, Ptr<VtkStressStrain>>(m, "VtkStressStrain",
                                                    DOC(amitex, VtkStressStrain))
      .def(py::init(&VtkStressStrain::create), "stress"_a = 0, "strain"_a = 0);

  py::class_<VtkFluxDGradD, Ptr<VtkFluxDGradD>>(m, "VtkFluxDGradD", DOC(amitex, VtkFluxDGradD))
      .def(py::init(&VtkFluxDGradD::create), "fluxd"_a = 0, "gradd"_a = 0);

  py::class_<Output, Ptr<Output>>(m, "Output", DOC(amitex, Output))
      .def(py::init(&Output::create))
      .def("setVtkStressStrain", &Output::setVtkStressStrain, "stress"_a, "strain"_a,
           DOC(amitex, Output, setVtkStressStrain))
      .def_readwrite("vtkStressStrain", &Output::vtkStressStrain,
                     DOC(amitex, Output, vtkStressStrain))
      .def("setVtkFluxDGradD", &Output::setVtkFluxDGradD, "fluxD"_a, "gradD"_a,
           DOC(amitex, Output, setVtkStressStrain))
      .def_readwrite("vtkFluxDGradD", &Output::vtkFluxDGradD, DOC(amitex, Output, vtkFluxDGradD))
      .def("addZone",
           static_cast<void (Output::*)(size_t, const std::vector<size_t>&)>(&Output::addZone),
           "numM"_a, "intVarList"_a = std::vector<size_t>{}, DOC(amitex, Output, addZone))
      .def("addVtkIntVarList", &Output::addVtkIntVarList, DOC(amitex, Output, addVtkIntVarList));

  py::class_<InitLoadExt, Ptr<InitLoadExt>>(m, "InitLoadExt", DOC(amitex, InitLoadExt))
      .def(py::init(&InitLoadExt::create))
      .DEF_VALUE_PROPERTY(temperature, double, InitLoadExt)
      .def("setParam", &InitLoadExt::setParam, DOC(amitex, InitLoadExt, setParam));

  py::class_<LoadingOutput, Ptr<LoadingOutput>>(m, "LoadingOutput", DOC(amitex, LoadingOutput))
      .def(py::init(&LoadingOutput::create))
      .def("add", &LoadingOutput::add, DOC(amitex, LoadingOutput, add))
      .def_readwrite("output", &LoadingOutput::output, DOC(amitex, LoadingOutput, output))
      .def_readwrite("initLoadExt", &LoadingOutput::initLoadExt,
                     DOC(amitex, LoadingOutput, initLoadExt))
      .def("toXML", &toXML<LoadingOutput>);

  py::class_<Zone, Ptr<Zone>>(m, "Zone", DOC(amitex, Zone))
      .def(py::init(
               static_cast<Ptr<Zone> (*)(GridSize, const std::vector<GridPoint>&)>(&Zone::create)),
           "gridSize"_a, "positions"_a = std::vector<GridPoint>{}, DOC(amitex, Zone, Zone, 2))
      .def("add", static_cast<void (Zone::*)(GridPoint)>(&Zone::add), DOC(amitex, Zone, add))
      .def("add", static_cast<void (Zone::*)(GridLinPoint)>(&Zone::add), DOC(amitex, Zone, add, 2))
      .def(
          "add",
          [](Zone& zone, py::iterator it) {
            while (it != py::iterator::sentinel()) {
              GridPoint p = it->cast<GridPoint>();
              zone.add(p);
              ++it;
            }
          },
          "add voxels with an iterator on grid points")
      .def("numberVoxels", &Zone::numberVoxels, DOC(amitex, Zone, numberVoxels))
      .def("linearPositions", &Zone::linearPositions, DOC(amitex, Zone, linearPositions));

  py::class_<Material, Ptr<Material>>(m, "Material", DOC(amitex, Material))
      .def(py::init(&Material::create))
      .def("setLaw", &Material::setLaw, "law"_a, "lib"_a = "", DOC(amitex, Material, setLaw))
      .def("setLawK", &Material::setLawK, "law"_a, "lib"_a = "", DOC(amitex, Material, setLawK))
      .def_property("law", nullptr, [](Material& m, const std::string& s) { m.setLaw(s); })
      .def_property("lawK", nullptr, [](Material& m, const std::string& s) { m.setLawK(s); })
      .def("setCoeff", &Material::setCoeff, DOC(amitex, Material, setCoeff))
      .def("setCoeffZone", &Material::setCoeffZone, DOC(amitex, Material, setCoeffZone))
      .def("setCoeffZoneFromBin", &Material::setCoeffZoneFromBin,
           DOC(amitex, Material, setCoeffZoneFromBin))
      .def("setCoeffs", &Material::setCoeffs, DOC(amitex, Material, setCoeffs))
      .def("setCoeffK", &Material::setCoeffK, DOC(amitex, Material, setCoeffK))
      .def("setCoeffKZone", &Material::setCoeffKZone, DOC(amitex, Material, setCoeffKZone))
      .def("setCoeffKZoneFromBin", &Material::setCoeffKZoneFromBin,
           DOC(amitex, Material, setCoeffKZoneFromBin))
      .def("setCoeffKs", &Material::setCoeffKs, DOC(amitex, Material, setCoeffKs))
      .def("setCoeffComposite", &Material::setCoeffComposite,
           DOC(amitex, Material, setCoeffComposite))
      .def("setCoeffCompositeZone", &Material::setCoeffCompositeZone,
           DOC(amitex, Material, setCoeffCompositeZone))
      .def("setCoeffCompositeZoneFromBin", &Material::setCoeffCompositeZoneFromBin,
           DOC(amitex, Material, setCoeffCompositeZoneFromBin))
      .def("setCoeffComposites", &Material::setCoeffComposites,
           DOC(amitex, Material, setCoeffComposites))
      .def_property("coeffs", nullptr, &Material::setCoeffs, DOC(amitex, Material, coeffs))
      .def_property("coeffKs", nullptr, &Material::setCoeffKs, DOC(amitex, Material, coeffKs))
      .def_property("coeffComposites", nullptr, &Material::setCoeffComposites,
                    DOC(amitex, Material, coeffComposites))
      .def("setNumberCoeff", &Material::setNumberCoeff, DOC(amitex, Material, setNumberCoeff))
      .def("setNumberCoeffK", &Material::setNumberCoeffK, DOC(amitex, Material, setNumberCoeffK))
      .def("setNumberCoeffComposite", &Material::setNumberCoeffComposite,
           DOC(amitex, Material, setNumberCoeffComposite))
      .def("numberCoeff", &Material::numberCoeff, DOC(amitex, Material, numberCoeff))
      .def("numberCoeffK", &Material::numberCoeffK, DOC(amitex, Material, numberCoeffK))
      .def("numberCoeffComposite", &Material::numberCoeffComposite,
           DOC(amitex, Material, numberCoeffComposite))
      .def("addZone", &Material::addZone, "zone"_a, "coeffs"_a = std::vector<double>{},
           "coeffKs"_a = std::vector<double>{}, "coeffComposites"_a = std::vector<double>{},
           DOC(amitex, Material, addZone))
      .def("numberZones", &Material::numberZones, DOC(amitex, Material, numberZones))
      .def("numberIntVars", &Material::numberIntVars, DOC(amitex, Material, numberIntVars))
      .def("addIntVar", static_cast<void (Material::*)(double)>(&Material::addIntVar),
           DOC(amitex, Material, addIntVar))
      .def(
          "addIntVar", [](Material& mat, const Field<double>& f) { mat.addIntVar(f); },
          DOC(amitex, Material, addIntVar, 2))
      .def("setCoeffName", &Material::setCoeffName, DOC(amitex, Material, setCoeffName))
      .def("setCoeffKName", &Material::setCoeffKName, DOC(amitex, Material, setCoeffKName))
      .def("setCoeffCompositeName", &Material::setCoeffCompositeName,
           DOC(amitex, Material, setCoeffCompositeName))
      .def("setIntVarName", &Material::setIntVarName, DOC(amitex, Material, setIntVarName));

  py::class_<InterfaceGeometry>(m, "InterfaceGeometry", DOC(amitex, InterfaceGeometry))
      .def(py::init())
      .def_readwrite("normal", &InterfaceGeometry::normal, DOC(amitex, InterfaceGeometry, normal))
      .def_readwrite("tangent", &InterfaceGeometry::tangent,
                     DOC(amitex, InterfaceGeometry, tangent))
      .def_readwrite("surface", &InterfaceGeometry::surface,
                     DOC(amitex, InterfaceGeometry, surface));

  py::class_<Composite, Ptr<Composite>>(m, "Composite", DOC(amitex, Composite))
      .def(py::init(&Composite::create), "phases"_a, "law"_a = "",
           DOC(amitex, Composite, Composite))
      .def("addVoxel",
           static_cast<void (Composite::*)(GridLinPoint, const std::vector<double>&,
                                           const std::vector<InterfaceGeometry>&,
                                           const std::vector<size_t>&)>(&Composite::addVoxel),
           "position"_a, "phi"_a, "geom"_a = std::vector<InterfaceGeometry>{},
           "zones"_a = std::vector<size_t>{}, DOC(amitex, Composite, addVoxel))
      .def("setLaw", static_cast<void (Composite::*)(const std::string&)>(&Composite::setLaw),
           DOC(amitex, Composite, setLaw))
      .def_property("law", nullptr, &Composite::setLaw)
      .def("numberPhases", &Composite::numberPhases, DOC(amitex, Composite, numberPhases))
      .def("materialIndices", &Composite::materialIndices, DOC(amitex, Composite, materialIndices))
      .def("positions",
           static_cast<const std::vector<GridLinPoint>& (Composite::*)() const>(&Composite::positions),
           DOC(amitex, Composite, positions));

  py::class_<ReferenceMaterial, Ptr<ReferenceMaterial>>(m, "ReferenceMaterial",
                                                        DOC(amitex, ReferenceMaterial))
      .def(py::init(&ReferenceMaterial::create), "lambda0"_a, "mu0"_a,
           DOC(amitex, ReferenceMaterial, ReferenceMaterial));

  py::class_<ReferenceMaterialD, Ptr<ReferenceMaterialD>>(m, "ReferenceMaterialD",
                                                          DOC(amitex, ReferenceMaterial))
      .def(py::init(&ReferenceMaterialD::create), "K0"_a,
           DOC(amitex, ReferenceMaterialD, ReferenceMaterialD));

  py::class_<Materials, Ptr<Materials>>(m, "Materials", DOC(amitex, Materials))
      .def(py::init(&Materials::create))
      .def("add", static_cast<void (Materials::*)(Ptr<Material>)>(&Materials::add),
           DOC(amitex, Materials, add))
      .def("add", static_cast<void (Materials::*)(Ptr<Composite>)>(&Materials::add),
           DOC(amitex, Materials, add, 2))
      .def_readwrite("referenceMaterial", &Materials::referenceMaterial,
                     DOC(amitex, Materials, referenceMaterial))
      .def_readwrite("referenceMaterialD", &Materials::referenceMaterialD,
                     DOC(amitex, Materials, referenceMaterialD))
      .def("numberMaterials", &Materials::numberMaterials, DOC(amitex, Materials, numberMaterials))
      .def("numberComposites", &Materials::numberComposites,
           DOC(amitex, Materials, numberComposites))
      .def("toXML", &toXML<Materials>)
      // .def("material", [](Materials& m, size_t i) -> Material& { return m.material(i); })
      .def("material", &Materials::material, DOC(amitex, Materials, material))
      .def("composite", &Materials::composite, DOC(amitex, Materials, composite));

  py::class_<Grid>(m, "Grid", DOC(amitex, Grid))
      .def(py::init<GridSize, Vector3D>(), "dims"_a, "dx"_a, DOC(amitex, Grid, Grid, 2))
      .def("setPbc", &Grid::setPbc, DOC(amitex, Grid, setPbc))
      .def_property("pbc", nullptr, &Grid::setPbc)
      .def("setOrigin", &Grid::setOrigin, DOC(amitex, Grid, setOrigin))
      .def_property("origin", nullptr, &Grid::setOrigin)
      .def("totalSize", &Grid::totalSize, DOC(amitex, Grid, totalSize))
      .def("dims", &Grid::dims, DOC(amitex, Grid, dims))
      .def("voxelLengths", &Grid::voxelLengths, DOC(amitex, Grid, voxelLengths))
      .def("linearize", &Grid::linearize, DOC(amitex, Grid, linearize))
      .def("distance",
           static_cast<double (Grid::*)(const GridPoint& a, const GridPoint& b) const>(
               &Grid::distance),
           DOC(amitex, Grid, distance))
      .def("distance",
           static_cast<double (Grid::*)(const GridPoint& a, const Vector3D& b) const>(
               &Grid::distance),
           DOC(amitex, Grid, distance, 2))
      .def(
          "allPoints",
          [](Grid& grid) {
            auto it = grid.allPoints();
            return py::make_iterator(it.begin(), it.end());
          },
          DOC(amitex, Grid, allPoints))
      .def(
          "allLinPoints",
          [](Grid& grid) {
            auto it = grid.allLinPoints();
            return py::make_iterator(it.begin(), it.end());
          },
          DOC(amitex, Grid, allLinPoints));

  py::class_<Input, Ptr<Input>>(m, "Input", DOC(amitex, Input))
      .def(py::init(&Input::create),
           "note: AlgorithmParameters, Materials, LoadingOutput passed to constructor are moved "
           "internally "
           "for performance reasons, invalidating their content",
           "grid"_a, "algorithmParameters"_a, "materials"_a, "loadingOutput"_a)
      .def_readwrite("grid", &Input::grid, DOC(amitex, Input, grid))
      .def_readwrite("loadingOutput", &Input::loadingOutput, DOC(amitex, Input, loadingOutput))
      .def_readwrite("algorithmParameters", &Input::algorithmParameters,
                     DOC(amitex, Input, algorithmParameters))
      .def_readwrite("materials", &Input::materials, DOC(amitex, Input, materials))
      .def_readwrite("resultsDir", &Input::resultsDir, DOC(amitex, Input, resultsDir))
      .def("outputPrefix", &Input::outputPrefix, DOC(amitex, Input, outputPrefix))
      .def("generateFiles", &Input::generateFiles, DOC(amitex, Input, generateFiles))
      .def("generateAlgorithm", &Input::generateAlgorithm, DOC(amitex, Input, generateAlgorithm))
      .def("generateMaterials", &Input::generateMaterials, DOC(amitex, Input, generateMaterials))
      .def("generateLoadingOutput", &Input::generateLoadingOutput,
           DOC(amitex, Input, generateLoadingOutput))
      .def("generateMaterialVTK", &Input::generateMaterialVTK,
           DOC(amitex, Input, generateMaterialVTK))
      .def("generateZoneVTK", &Input::generateZoneVTK, DOC(amitex, Input, generateZoneVTK));
  m.attr("__version__") = AMITEX_VERSION;
}

}  // namespace amitex_python
