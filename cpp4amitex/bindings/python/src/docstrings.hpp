/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_amitex_Algorithm = R"doc(Main algorithm parameters)doc";

static const char *__doc_amitex_AlgorithmLaminate = R"doc(Special algorithm parameters for simulations with composite law (laminate, reuss, …))doc";

static const char *__doc_amitex_AlgorithmLaminate_AlgorithmLaminate = R"doc()doc";

static const char *__doc_amitex_AlgorithmLaminate_convergenceAcceleration = R"doc(Toggle convergence acceleration)doc";

static const char *__doc_amitex_AlgorithmLaminate_convergenceCriterion = R"doc(Convergence criterion (>1e-4 and >1e-1))doc";

static const char *__doc_amitex_AlgorithmLaminate_initializationType = R"doc(Initialization type (Proportionnal, Linear, Default (=Linear))doc";

static const char *__doc_amitex_AlgorithmLaminate_nIncrements = R"doc(Number of substeps for laminate law)doc";

static const char *__doc_amitex_AlgorithmLaminate_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_AlgorithmLaminate_xmlTag = R"doc()doc";

static const char *__doc_amitex_AlgorithmLaminate_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_AlgorithmLaminate_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters = R"doc(Group of all algorithm parameters)doc";

static const char *__doc_amitex_AlgorithmParameters_AlgorithmParameters = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_AlgorithmParameters_2 = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_algorithm = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_algorithmLaminate = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_diffusion = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_mechanics = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_xmlTag = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_AlgorithmParameters_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Algorithm_Algorithm = R"doc()doc";

static const char *__doc_amitex_Algorithm_Algorithm_2 = R"doc()doc";

static const char *__doc_amitex_Algorithm_convergenceAcceleration = R"doc(Toggle use of convergence acceleration)doc";

static const char *__doc_amitex_Algorithm_convergenceCriterion = R"doc(convergence criterion (<1.e-3))doc";

static const char *__doc_amitex_Algorithm_convergenceCriterionCompatibility = R"doc(compatibility criterion)doc";

static const char *__doc_amitex_Algorithm_convergenceCriterionSmacro = R"doc(Convergence criterion for macroscropic applied stress default value is the one used for convergenceCriterion)doc";

static const char *__doc_amitex_Algorithm_convergenceForced = R"doc(Force convergence)doc";

static const char *__doc_amitex_Algorithm_initialize = R"doc(Pre-step initialization ("default" or "previous"))doc";

static const char *__doc_amitex_Algorithm_nitermax = R"doc(max number of iteration)doc";

static const char *__doc_amitex_Algorithm_nitermin = R"doc(minimum number of iterations (0 or 1))doc";

static const char *__doc_amitex_Algorithm_niterminACV = R"doc(minimum number of iterations (0 or 1) after each accelerated solution)doc";

static const char *__doc_amitex_Algorithm_substepping = R"doc(substepping)doc";

static const char *__doc_amitex_Algorithm_type = R"doc(Type ("Dafault" or "Basic_Scheme"))doc";

static const char *__doc_amitex_Algorithm_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Algorithm_xmlTag = R"doc()doc";

static const char *__doc_amitex_Algorithm_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Algorithm_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_AmitexError = R"doc(Thrown by execution of AMITEX-FFTP **warning**:  Trying to continue a simulation (e.g. a new timestep) afterwards is undefined)doc";

static const char *__doc_amitex_AmitexError_AmitexError = R"doc()doc";

static const char *__doc_amitex_AmitexError_AmitexError_2 = R"doc()doc";

static const char *__doc_amitex_AmitexError_code = R"doc()doc";

static const char *__doc_amitex_AmitexError_msg = R"doc()doc";

static const char *__doc_amitex_AmitexError_operator_assign = R"doc()doc";

static const char *__doc_amitex_AmitexError_what = R"doc()doc";

static const char *__doc_amitex_BasicCoeff = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_BasicCoeff = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_BasicCoeff_2 = R"doc(Define a coefficient by a constant value)doc";

static const char *__doc_amitex_BasicCoeff_BasicCoeff_3 =
R"doc(Define a coefficient by a constant value per zone

Parameter ``values``:
    list of values by increasing zone index **note**: It is recommanded to use Material::addZone for this purpose)doc";

static const char *__doc_amitex_BasicCoeff_BasicCoeff_4 = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_constantValue =
R"doc(Returns:
    the value of the coefficient)doc";

static const char *__doc_amitex_BasicCoeff_constantValue_2 = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_file = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_id = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_name = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_setFile = R"doc(set path of coefficient file. In general you do not need to call it.)doc";

static const char *__doc_amitex_BasicCoeff_setIndex = R"doc(set index (corresponding to law). In general you do not need to call it.)doc";

static const char *__doc_amitex_BasicCoeff_setName = R"doc(Attach a name to a coefficient (optional))doc";

static const char *__doc_amitex_BasicCoeff_values = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_xmlTag = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_BasicCoeff_zoneValues =
R"doc(Returns:
    the value per zone of the coefficient)doc";

static const char *__doc_amitex_BasicCoeff_zoneValues_2 = R"doc()doc";

static const char *__doc_amitex_Coeff = R"doc()doc";

static const char *__doc_amitex_CoeffComposite = R"doc()doc";

static const char *__doc_amitex_CoeffComposite_xmlTag = R"doc()doc";

static const char *__doc_amitex_CoeffK = R"doc()doc";

static const char *__doc_amitex_CoeffK_xmlTag = R"doc()doc";

static const char *__doc_amitex_Coeff_xmlTag = R"doc()doc";

static const char *__doc_amitex_Component = R"doc(Loading axis pair and axis **note**: only finite strain needs YX,ZX, ZY)doc";

static const char *__doc_amitex_Composite = R"doc(Composite material **note**: the coefficients are set in the pure material (for ex Material::setCoeffComposite))doc";

static const char *__doc_amitex_Composite_Composite =
R"doc(Parameter ``materialIndices``:
    indices of pure materials (as defined for Materials) of each phase

Parameter ``law``:
    averaging law)doc";

static const char *__doc_amitex_Composite_N = R"doc()doc";

static const char *__doc_amitex_Composite_S = R"doc()doc";

static const char *__doc_amitex_Composite_T = R"doc()doc";

static const char *__doc_amitex_Composite_addVoxel =
R"doc(add a voxel to the material

Parameter ``position``:
    linearized position

Parameter ``phi``:
    volume fractions of each phase)doc";

static const char *__doc_amitex_Composite_addVoxel_2 =
R"doc(add a voxel to the material

Parameter ``position``:
    linearized position

Parameter ``phi``:
    volume fractions of each phase

Parameter ``geom``:
    interface geometry for each couple of phases (order i<j: 00 01 …))doc";

static const char *__doc_amitex_Composite_law = R"doc(Get composite law)doc";

static const char *__doc_amitex_Composite_law_2 = R"doc()doc";

static const char *__doc_amitex_Composite_materialIndices = R"doc(Get the material indices for each phase)doc";

static const char *__doc_amitex_Composite_normals = R"doc(Get normals)doc";

static const char *__doc_amitex_Composite_numberPhases = R"doc(Get number of phases)doc";

static const char *__doc_amitex_Composite_phaseIndices = R"doc()doc";

static const char *__doc_amitex_Composite_pos = R"doc()doc";

static const char *__doc_amitex_Composite_positions = R"doc(Get the linearized positions of voxels constituting the material)doc";

static const char *__doc_amitex_Composite_positions_2 = R"doc()doc";

static const char *__doc_amitex_Composite_setLaw = R"doc(Set averaging law)doc";

static const char *__doc_amitex_Composite_surfaces = R"doc(Get surfaces)doc";

static const char *__doc_amitex_Composite_tangents = R"doc(Get tangents)doc";

static const char *__doc_amitex_Composite_volfracs = R"doc()doc";

static const char *__doc_amitex_Composite_volumeFractions =
R"doc(Get the volume fractions of a phase

Parameter ``index``:
    phase index)doc";

static const char *__doc_amitex_Composite_volumeFractions_2 = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration = R"doc(Convergence accelaration setting(s))doc";

static const char *__doc_amitex_ConvergenceAcceleration_ConvergenceAcceleration = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_ConvergenceAcceleration_2 = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_ConvergenceAcceleration_3 = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_ConvergenceAcceleration_4 = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_modACV = R"doc(number of iterations between two convergence accelerations)doc";

static const char *__doc_amitex_ConvergenceAcceleration_operator_assign = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_operator_bool = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_value = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_xmlTag = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_ConvergenceAcceleration_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced = R"doc(Force convergence)doc";

static const char *__doc_amitex_ConvergenceForced_ConvergenceForced = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_ConvergenceForced_2 = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_nCVFor = R"doc(TODO: Document)doc";

static const char *__doc_amitex_ConvergenceForced_nitCVFor = R"doc(TODO: Document)doc";

static const char *__doc_amitex_ConvergenceForced_operator_assign = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_operator_bool = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_value = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_xmlTag = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_ConvergenceForced_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Diffusion = R"doc()doc";

static const char *__doc_amitex_DiffusionDriving = R"doc(Types of driving for difffusion problems)doc";

static const char *__doc_amitex_DiffusionDriving_Flux = R"doc()doc";

static const char *__doc_amitex_DiffusionDriving_Gradient = R"doc()doc";

static const char *__doc_amitex_Diffusion_Diffusion = R"doc()doc";

static const char *__doc_amitex_Diffusion_Diffusion_2 = R"doc()doc";

static const char *__doc_amitex_Diffusion_filter = R"doc()doc";

static const char *__doc_amitex_Diffusion_stationary = R"doc()doc";

static const char *__doc_amitex_Diffusion_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Diffusion_xmlTag = R"doc()doc";

static const char *__doc_amitex_Diffusion_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Diffusion_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_DirStress = R"doc(Type of stress tensor)doc";

static const char *__doc_amitex_DirStress_Cauchy = R"doc()doc";

static const char *__doc_amitex_DirStress_PK1 = R"doc()doc";

static const char *__doc_amitex_Evolution = R"doc(Types of loading evolution)doc";

static const char *__doc_amitex_Evolution_Constant = R"doc()doc";

static const char *__doc_amitex_Evolution_Linear = R"doc()doc";

static const char *__doc_amitex_Extract = R"doc()doc";

static const char *__doc_amitex_Extract_DiffusionQuantities = R"doc()doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_flux = R"doc()doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_gradient = R"doc()doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_niter = R"doc(number of iterations)doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_sigFlux = R"doc(standard deviations)doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_sigGradient = R"doc(standard deviations)doc";

static const char *__doc_amitex_Extract_DiffusionQuantities_t = R"doc(time)doc";

static const char *__doc_amitex_Extract_Extract = R"doc()doc";

static const char *__doc_amitex_Extract_Extract_2 = R"doc()doc";

static const char *__doc_amitex_Extract_MechanicalQuantities = R"doc()doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_niter = R"doc(number of iterations)doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_sigStrain = R"doc(standard deviations)doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_sigStress = R"doc(standard deviations)doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_strain = R"doc()doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_stress = R"doc()doc";

static const char *__doc_amitex_Extract_MechanicalQuantities_t = R"doc(time)doc";

static const char *__doc_amitex_Extract_averageDiffusionFlux =
R"doc(Returns:
    the last computed diffusion flux)doc";

static const char *__doc_amitex_Extract_averageDiffusionGradient =
R"doc(Returns:
    the last computed diffusion gradient)doc";

static const char *__doc_amitex_Extract_averageStrain =
R"doc(Returns:
    the last computed strain (indexing: XX YY ZZ XY XZ YZ YZ ZX ZY))doc";

static const char *__doc_amitex_Extract_averageStress =
R"doc(Returns:
    the last computed stress (indexing: XX YY ZZ XY XZ YZ YZ ZX ZY))doc";

static const char *__doc_amitex_Extract_currentDiffusion = R"doc()doc";

static const char *__doc_amitex_Extract_currentMechanics = R"doc()doc";

static const char *__doc_amitex_Extract_lastWrite = R"doc()doc";

static const char *__doc_amitex_Extract_prefix = R"doc()doc";

static const char *__doc_amitex_Extract_readStd = R"doc()doc";

static const char *__doc_amitex_Extract_readStd_2 = R"doc()doc";

static const char *__doc_amitex_Extract_setSymComponents = R"doc()doc";

static const char *__doc_amitex_Extract_toTensor3D = R"doc()doc";

static const char *__doc_amitex_Field =
R"doc(3D Field template class

The internal data is indexed by Fortran convention (ie left-most index is first in memory)

Optional bound-checked accessor (x(i,j,k) x[{i,j,k}]) is off when NDEBUG is defined

Assignment does not copy the underlying buffer, generate a fresh copy with the method ``copy()`` instead)doc";

static const char *__doc_amitex_Field_Field = R"doc()doc";

static const char *__doc_amitex_Field_Field_2 =
R"doc(Field on the whole grid

Parameter ``gridDims``:
    grid dimensions)doc";

static const char *__doc_amitex_Field_Field_3 =
R"doc(Field on the whole grid, non-owning memory

Parameter ``gridDims``:
    grid dimensions

Parameter ``buffer``:)doc";

static const char *__doc_amitex_Field_Field_4 =
R"doc(Field on a rectangular region

Parameter ``gridDims``:
    grid dimensions

Parameter ``ibegin``:
    beginning of the rane in grid coordinates (*inclusive*)

Parameter ``iend``:
    end of the rane in grid coordinates (*exclusive*))doc";

static const char *__doc_amitex_Field_at = R"doc(get data at grid point with bound checking)doc";

static const char *__doc_amitex_Field_at_2 = R"doc()doc";

static const char *__doc_amitex_Field_bounds = R"doc(Get range [begin, end) of accessible coordinates)doc";

static const char *__doc_amitex_Field_checkBounds = R"doc()doc";

static const char *__doc_amitex_Field_copy = R"doc(Create a new instance with a copy of the internal data)doc";

static const char *__doc_amitex_Field_data = R"doc(get underlying data buffer)doc";

static const char *__doc_amitex_Field_data_2 = R"doc()doc";

static const char *__doc_amitex_Field_dataPtr = R"doc()doc";

static const char *__doc_amitex_Field_dims = R"doc(Get dimensions)doc";

static const char *__doc_amitex_Field_empty = R"doc(check if no data allocated)doc";

static const char *__doc_amitex_Field_fill = R"doc(Fill all the (owned) space with `value`.)doc";

static const char *__doc_amitex_Field_ibegin = R"doc()doc";

static const char *__doc_amitex_Field_iend = R"doc()doc";

static const char *__doc_amitex_Field_inBounds = R"doc(Check if `p` is in bounds)doc";

static const char *__doc_amitex_Field_inBounds_2 = R"doc(Check if {`ix`, `iy`, `iz`} is in bounds)doc";

static const char *__doc_amitex_Field_lbound = R"doc(Get lower bound (inclusive))doc";

static const char *__doc_amitex_Field_loadFromVtk =
R"doc(Create a field from a VTK file

Parameter ``path``:
    file path)doc";

static const char *__doc_amitex_Field_managed =
R"doc(Returns:
    `true` if the underlying data is managed)doc";

static const char *__doc_amitex_Field_nx = R"doc()doc";

static const char *__doc_amitex_Field_operator_array =
R"doc(get data at grid point with optional bound checking

Parameter ``p``:
    grid coordinates)doc";

static const char *__doc_amitex_Field_operator_array_2 = R"doc()doc";

static const char *__doc_amitex_Field_operator_call = R"doc(get data at grid point with optional bound checking)doc";

static const char *__doc_amitex_Field_operator_call_2 = R"doc()doc";

static const char *__doc_amitex_Field_size = R"doc(Get total size)doc";

static const char *__doc_amitex_Field_storage = R"doc()doc";

static const char *__doc_amitex_Field_ubound = R"doc(Get upper bound (exclusive))doc";

static const char *__doc_amitex_Field_uncheckedAt = R"doc(get data at grid point without bound checking)doc";

static const char *__doc_amitex_Field_uncheckedAt_2 = R"doc()doc";

static const char *__doc_amitex_Grid = R"doc(Unit cell geometry (sizes, lengths...))doc";

static const char *__doc_amitex_Grid_AllLinPoints = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_AllLinPoints = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_Iterator = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_Iterator_Iterator = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_Iterator_current = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_Iterator_dims = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_Iterator_operator_mul = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_begin = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_dims = R"doc()doc";

static const char *__doc_amitex_Grid_AllLinPoints_end = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_AllPoints = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_Iterator = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_current = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_dims = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_operator_eq = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_operator_inc = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_operator_mul = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_Iterator_operator_ne = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_begin = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_dims = R"doc()doc";

static const char *__doc_amitex_Grid_AllPoints_end = R"doc()doc";

static const char *__doc_amitex_Grid_Grid = R"doc()doc";

static const char *__doc_amitex_Grid_Grid_2 = R"doc()doc";

static const char *__doc_amitex_Grid_allLinPoints =
R"doc(Returns:
    an iterable over all points (linear representation) on the grid \example

```
C++
for(GridLinPoint p : grid.allLinPoints()) …
```)doc";

static const char *__doc_amitex_Grid_allPoints =
R"doc(Returns:
    an iterable over all points on the grid \example

```
C++
for(GridPoint p : grid.allPoints()) …
```)doc";

static const char *__doc_amitex_Grid_dims = R"doc(Per-axis number of voxels)doc";

static const char *__doc_amitex_Grid_dims_2 = R"doc()doc";

static const char *__doc_amitex_Grid_distance = R"doc(Compute distance between two points)doc";

static const char *__doc_amitex_Grid_distance_2 = R"doc()doc";

static const char *__doc_amitex_Grid_getPartition = R"doc(Get global ranges of voxel coordinates)doc";

static const char *__doc_amitex_Grid_linearize =
R"doc(Get linearized position

Parameter ``p``:
    position in grid coordinates)doc";

static const char *__doc_amitex_Grid_origin = R"doc()doc";

static const char *__doc_amitex_Grid_pbc = R"doc()doc";

static const char *__doc_amitex_Grid_setOrigin = R"doc()doc";

static const char *__doc_amitex_Grid_setPbc = R"doc()doc";

static const char *__doc_amitex_Grid_totalSize = R"doc(Total number of voxels)doc";

static const char *__doc_amitex_Grid_voxelLengths = R"doc(Lengths of a voxel)doc";

static const char *__doc_amitex_Grid_voxelLengths_2 = R"doc()doc";

static const char *__doc_amitex_IndexOrdering = R"doc(Order or elements in memory for multidimentional arrays represented by vector<> or, alternatively, order of linearized positions)doc";

static const char *__doc_amitex_IndexOrdering_C = R"doc()doc";

static const char *__doc_amitex_IndexOrdering_Fortran = R"doc()doc";

static const char *__doc_amitex_InitLoadExt = R"doc(Initialization of the temperature and the external parameters **warning**:  If defined, the evolution of the temperature and all the parameters must be specified in the loadings.)doc";

static const char *__doc_amitex_InitLoadExt_InitLoadExt = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_Param = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_index = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_value = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_xmlTag = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_Param_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_params = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_setParam =
R"doc(Set parameter

Parameter ``index``:
    Index of parameter)doc";

static const char *__doc_amitex_InitLoadExt_temperature = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_xmlTag = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_InitLoadExt_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Input = R"doc(Regroup all AMITEX input parameters)doc";

static const char *__doc_amitex_InputError = R"doc(Thrown by input parametrizing)doc";

static const char *__doc_amitex_InputError_InputError = R"doc()doc";

static const char *__doc_amitex_InputError_msg = R"doc()doc";

static const char *__doc_amitex_InputError_operator_assign = R"doc()doc";

static const char *__doc_amitex_InputError_what = R"doc()doc";

static const char *__doc_amitex_Input_Input = R"doc()doc";

static const char *__doc_amitex_Input_Input_2 = R"doc()doc";

static const char *__doc_amitex_Input_Input_3 = R"doc()doc";

static const char *__doc_amitex_Input_Input_4 = R"doc()doc";

static const char *__doc_amitex_Input_Input_5 = R"doc()doc";

static const char *__doc_amitex_Input_algorithmParameters = R"doc(see AlgorithmParameters)doc";

static const char *__doc_amitex_Input_algorithmPath = R"doc(Path to XML defining algorithm parameters)doc";

static const char *__doc_amitex_Input_generateFiles = R"doc(Generate all input files (XML, VTK, BIN) in resultsDir)doc";

static const char *__doc_amitex_Input_grid = R"doc(Grid defining the unit cell and number of voxels)doc";

static const char *__doc_amitex_Input_loadingOutput = R"doc(see LoadingOutput)doc";

static const char *__doc_amitex_Input_loadingPath = R"doc(Path to XML file defining loading and output)doc";

static const char *__doc_amitex_Input_materialIdsPath = R"doc(Path to VTK file defining material placement)doc";

static const char *__doc_amitex_Input_materials = R"doc(see Materials)doc";

static const char *__doc_amitex_Input_materialsPath = R"doc(Path to XML file defining materials)doc";

static const char *__doc_amitex_Input_outputPrefix = R"doc(Prefix to output files)doc";

static const char *__doc_amitex_Input_resultsDir = R"doc(Where generated and AMITEX output will be)doc";

static const char *__doc_amitex_Input_zoneIdsPath = R"doc(Path to VTK file defining zones)doc";

static const char *__doc_amitex_IntVar = R"doc(Internal variable)doc";

static const char *__doc_amitex_IntVar_IntVar = R"doc()doc";

static const char *__doc_amitex_IntVar_IntVar_2 = R"doc()doc";

static const char *__doc_amitex_IntVar_IntVar_3 = R"doc()doc";

static const char *__doc_amitex_IntVar_IntVar_4 = R"doc()doc";

static const char *__doc_amitex_IntVar_IntVar_5 = R"doc()doc";

static const char *__doc_amitex_IntVar_field = R"doc(values as a function of grid coordinates)doc";

static const char *__doc_amitex_IntVar_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_IntVar_xmlTag = R"doc()doc";

static const char *__doc_amitex_IntVar_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_IntVar_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_InterfaceGeometry = R"doc(Define an interface in a composite voxel)doc";

static const char *__doc_amitex_InterfaceGeometry_normal = R"doc()doc";

static const char *__doc_amitex_InterfaceGeometry_surface = R"doc()doc";

static const char *__doc_amitex_InterfaceGeometry_tangent = R"doc()doc";

static const char *__doc_amitex_Interphase = R"doc(Definition of an "interphase" material. **note**: Used for input file generation, from material and composite definitions, generally of not to be use directly.)doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_nZones = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_numM = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_xmlTag = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseMaterial_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_nZones = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_numM = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_xmlTag = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Interphase_InterphaseZoneList_zoneList = R"doc()doc";

static const char *__doc_amitex_Interphase_addMaterial =
R"doc(add a material to the list of interphases

Parameter ``index``:
    index of the material

Parameter ``nZones``:
    number of zones)doc";

static const char *__doc_amitex_Interphase_addZones =
R"doc(add a zone of a material to the list of interphases

Parameter ``materialId``:
    index of the material

Parameter ``zones``:
    indices of zones)doc";

static const char *__doc_amitex_Interphase_materials = R"doc()doc";

static const char *__doc_amitex_Interphase_numberMaterials = R"doc()doc";

static const char *__doc_amitex_Interphase_numberZoneLists = R"doc()doc";

static const char *__doc_amitex_Interphase_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Interphase_xmlTag = R"doc()doc";

static const char *__doc_amitex_Interphase_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Interphase_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Interphase_zoneLists = R"doc()doc";

static const char *__doc_amitex_List = R"doc(Helper class to define elements containing space-separated list of values (like <ELEMENT>value0 value1 … </ELEMENT>))doc";

static const char *__doc_amitex_List_List = R"doc()doc";

static const char *__doc_amitex_List_add = R"doc()doc";

static const char *__doc_amitex_List_add_2 = R"doc()doc";

static const char *__doc_amitex_List_numberValues = R"doc(Get the number of listed values)doc";

static const char *__doc_amitex_List_tag = R"doc()doc";

static const char *__doc_amitex_List_values = R"doc()doc";

static const char *__doc_amitex_List_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_List_xmlTag = R"doc()doc";

static const char *__doc_amitex_List_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_List_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading = R"doc((partial) loading)doc";

static const char *__doc_amitex_LoadingOutput = R"doc(Definitions of all loadings and output settings)doc";

static const char *__doc_amitex_LoadingOutput_LoadingOutput = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_add = R"doc(Append a partial loading)doc";

static const char *__doc_amitex_LoadingOutput_add_2 = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_initLoadExt = R"doc(Initialization of the temperature and the external parameters)doc";

static const char *__doc_amitex_LoadingOutput_loadings = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_output = R"doc(Output specification)doc";

static const char *__doc_amitex_LoadingOutput_setLastId = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_xmlTag = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_LoadingOutput_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_DiffusionDriver = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_component = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_driving = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_evolution = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_tag = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_value = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_DiffusionDriver_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_DirStress = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_DirStress_2 = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_dirstress = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_DirStress_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU = R"doc(Component of a gradient of gradient of displacement)doc";

static const char *__doc_amitex_Loading_GradGradU_GradGradU = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_evolution = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_tag = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_tcomponent = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_value = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_vcomponent = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_GradGradU_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_MechanicsDriver = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_component = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_dirstress = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_driving = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_evolution = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_tag = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_value = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_MechanicsDriver_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_OutputNumber = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_OutputNumber_2 = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_number = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_tag = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_OutputNumber_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_Param = R"doc()doc";

static const char *__doc_amitex_Loading_Param_Param = R"doc()doc";

static const char *__doc_amitex_Loading_Param_evolution = R"doc()doc";

static const char *__doc_amitex_Loading_Param_index = R"doc()doc";

static const char *__doc_amitex_Loading_Param_value = R"doc()doc";

static const char *__doc_amitex_Loading_Param_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_Param_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_Param_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_Param_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_Temperature = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_evolution = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_value = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_Temperature_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_TimeDiscretisation = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_TimeDiscretisation_2 = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_discretization = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_nincr = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_tfinal = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_TimeDiscretisation_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Loading_addUserInterruptValue = R"doc(Add a value for user-defined interuptions)doc";

static const char *__doc_amitex_Loading_diffu_drivers = R"doc()doc";

static const char *__doc_amitex_Loading_dirstress = R"doc()doc";

static const char *__doc_amitex_Loading_gradGradUDrivers = R"doc()doc";

static const char *__doc_amitex_Loading_id = R"doc()doc";

static const char *__doc_amitex_Loading_meca_drivers = R"doc()doc";

static const char *__doc_amitex_Loading_outputCell = R"doc()doc";

static const char *__doc_amitex_Loading_outputVtkList = R"doc()doc";

static const char *__doc_amitex_Loading_outputZone = R"doc()doc";

static const char *__doc_amitex_Loading_params = R"doc()doc";

static const char *__doc_amitex_Loading_setConstantEvolution =
R"doc(set a diffusion constant loading for one component

Parameter ``component``:
    $Parameter ``driving``:)doc";

static const char *__doc_amitex_Loading_setConstantEvolution_2 =
R"doc(set a mechanical constant loading for one component

Parameter ``component``:
    $Parameter ``driving``:

Parameter ``dirstress``:
    stress direction)doc";

static const char *__doc_amitex_Loading_setDirStress =
R"doc(Set the constant stress direction behavior for finite strains

Parameter ``dirStress``:
    type of stress tensor)doc";

static const char *__doc_amitex_Loading_setEvolution =
R"doc(set a diffusion loading evolution for one component

Parameter ``component``:
    $Parameter ``driving``:

Parameter ``evolution``:
    $Parameter ``value``:

value for linear evolution

Parameter ``dirstress``:
    stress direction)doc";

static const char *__doc_amitex_Loading_setEvolution_2 =
R"doc(set a mechanical loading evolution for one component

Parameter ``component``:
    $Parameter ``driving``:

Parameter ``evolution``:
    $Parameter ``value``:

final value for linear evolution

Parameter ``dirstress``:
    stress direction)doc";

static const char *__doc_amitex_Loading_setGradGradU =
R"doc(Set the evolution and value of a component grad_i grad_j U_k of an applied strain gradient (bending/twisting beam) component

Parameter ``ij``:
    directions of gradients

Parameter ``k``:
    axis of displacement

Parameter ``evolution``:
    Type of evolution

Parameter ``value``:
    Value for linear evolution \todo precise the meaning of components)doc";

static const char *__doc_amitex_Loading_setIndex = R"doc(Set the index (or tag))doc";

static const char *__doc_amitex_Loading_setLinearEvolution =
R"doc(set a diffusion constant loading for one component

Parameter ``component``:
    $Parameter ``driving``:

Parameter ``value``:
    final loading value along `component`)doc";

static const char *__doc_amitex_Loading_setLinearEvolution_2 =
R"doc(set a mechanical constant loading for one component

Parameter ``component``:
    $Parameter ``driving``:

Parameter ``value``:
    final loading value along `component`

Parameter ``dirstress``:
    stress direction)doc";

static const char *__doc_amitex_Loading_setOutputCell = R"doc(Set the number of increments where per-unit cell quantities are output)doc";

static const char *__doc_amitex_Loading_setOutputVtkList =
R"doc(Set the list of increment numbers where to ouput

Parameter ``increments``:
    list of increment numbers)doc";

static const char *__doc_amitex_Loading_setOutputZone = R"doc(Set the number of increments where zone quantities are output)doc";

static const char *__doc_amitex_Loading_setParamEvolution =
R"doc(set a the evolution of an external loading parameter

Parameter ``index``:
    index of parameter (defined in InitLoadExt)

Parameter ``evolution``:
    $Parameter ``value``:

value for linear evolution)doc";

static const char *__doc_amitex_Loading_setTemperatureEvolution =
R"doc(set a the evolution of the temperature

Parameter ``index``:
    index of parameter (defined in InitLoadExt)

Parameter ``evolution``:
    $Parameter ``value``:

value for linear evolution)doc";

static const char *__doc_amitex_Loading_setTimeDiscretizationLinear =
R"doc(Discretize time by a constant interval

Parameter ``increments``:
    the number of timesteps

Parameter ``tFinal``:
    final time)doc";

static const char *__doc_amitex_Loading_setTimeDiscretizationUser =
R"doc(Discretize time by a supplying a list of timesteps

Parameter ``increments``:
    the number of timesteps

Parameter ``times``:
    pointer to the array of times)doc";

static const char *__doc_amitex_Loading_setTimeDiscretizationUser_2 =
R"doc(Discretize time by a supplying a list of timesteps

Parameter ``times``:
    list of times)doc";

static const char *__doc_amitex_Loading_setUserInterruptValues = R"doc(Add a list of values for user-defined interuptions)doc";

static const char *__doc_amitex_Loading_tdisc = R"doc()doc";

static const char *__doc_amitex_Loading_temperature = R"doc()doc";

static const char *__doc_amitex_Loading_tlist = R"doc()doc";

static const char *__doc_amitex_Loading_userItpts = R"doc()doc";

static const char *__doc_amitex_Loading_userTimeList = R"doc()doc";

static const char *__doc_amitex_Loading_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Loading_xmlTag = R"doc()doc";

static const char *__doc_amitex_Loading_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Loading_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Material = R"doc(Definition of a "pure" material)doc";

static const char *__doc_amitex_MaterialBuilder =
R"doc(Higher-level contruction of materials from voxel specification

**note**: This module is concerned with construction of zones and composite specification, the user must specify the material behavior (law, coefficient))doc";

static const char *__doc_amitex_MaterialBuilder_MaterialBuilder = R"doc()doc";

static const char *__doc_amitex_MaterialBuilder_MaterialBuilder_2 =
R"doc(Parameter ``minVolFrac``:
    minimum volume fraction for a phase to be included (for more ion this problem see AMITEX doc for composites))doc";

static const char *__doc_amitex_MaterialBuilder_addVoxel =
R"doc(Add a voxel with phases of (num_material, volume phraction)

Parameter ``materials``:
    where the voxel will be added

Parameter ``pos``:
    linear position

Parameter ``spec``:
    voxel specification

Parameter ``normal``:
    normal of the interface)doc";

static const char *__doc_amitex_MaterialBuilder_addVoxel_2 =
R"doc(Add a non-composite voxel

Parameter ``numM``:
    index of material)doc";

static const char *__doc_amitex_MaterialBuilder_addVoxelAux = R"doc()doc";

static const char *__doc_amitex_MaterialBuilder_compos = R"doc()doc";

static const char *__doc_amitex_MaterialBuilder_minVolFrac = R"doc()doc";

static const char *__doc_amitex_MaterialComposite = R"doc(Define all composite materials)doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite_directory = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite_xmlTag = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_CoeffComposite_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_MaterialComposite = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_add = R"doc(Add a composite material)doc";

static const char *__doc_amitex_MaterialComposite_add_2 = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_at =
R"doc(Get a composite material

Parameter ``index``:
    of the composite material)doc";

static const char *__doc_amitex_MaterialComposite_at_2 = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_coeffComposite = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_materials = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_numberMaterials = R"doc(Get number of defined composite materials)doc";

static const char *__doc_amitex_MaterialComposite_setDirectory = R"doc(Set directory where generated files wil be located)doc";

static const char *__doc_amitex_MaterialComposite_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_xmlTag = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_MaterialComposite_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Material_Material = R"doc()doc";

static const char *__doc_amitex_Material_Material_2 = R"doc()doc";

static const char *__doc_amitex_Material_addIntVar =
R"doc(Add an internal variable

Parameter ``field``:
    initial values of varaible)doc";

static const char *__doc_amitex_Material_addIntVar_2 = R"doc()doc";

static const char *__doc_amitex_Material_addIntVar_3 = R"doc()doc";

static const char *__doc_amitex_Material_addZone =
R"doc(define a zone

Parameter ``zone``:
    voxel grid coordinates

Parameter ``coeffs``:
    mechanics coefficients

Parameter ``coeffKs``:
    diffusion coefficients)doc";

static const char *__doc_amitex_Material_addZone_2 = R"doc()doc";

static const char *__doc_amitex_Material_coeff =
R"doc(Returns:
    reference to mechanics coefficient

Parameter ``id``:
    index of coefficient)doc";

static const char *__doc_amitex_Material_coeff_2 = R"doc()doc";

static const char *__doc_amitex_Material_coeffComposite =
R"doc(Returns:
    reference to mechanics coefficient (value in composite)

Parameter ``id``:
    index of coefficient)doc";

static const char *__doc_amitex_Material_coeffComposite_2 = R"doc()doc";

static const char *__doc_amitex_Material_coeffComposites = R"doc()doc";

static const char *__doc_amitex_Material_coeffK =
R"doc(Returns:
    reference to diffusion coefficient

Parameter ``id``:
    index of coefficient)doc";

static const char *__doc_amitex_Material_coeffK_2 = R"doc()doc";

static const char *__doc_amitex_Material_coeffKs = R"doc()doc";

static const char *__doc_amitex_Material_coeffs = R"doc()doc";

static const char *__doc_amitex_Material_id = R"doc()doc";

static const char *__doc_amitex_Material_intVar =
R"doc(Get internal variable

Parameter ``id``:
    index of variable)doc";

static const char *__doc_amitex_Material_intVar_2 = R"doc()doc";

static const char *__doc_amitex_Material_interphase = R"doc(Interphase material appear nowhere 'pure'.)doc";

static const char *__doc_amitex_Material_intvars = R"doc()doc";

static const char *__doc_amitex_Material_law = R"doc()doc";

static const char *__doc_amitex_Material_lawK = R"doc()doc";

static const char *__doc_amitex_Material_lib = R"doc()doc";

static const char *__doc_amitex_Material_libK = R"doc()doc";

static const char *__doc_amitex_Material_nbZones = R"doc()doc";

static const char *__doc_amitex_Material_numberCoeff =
R"doc(Returns:
    the number of mechanics coefficient)doc";

static const char *__doc_amitex_Material_numberCoeffComposite =
R"doc(Returns:
    the number of diffusion coefficient (composite))doc";

static const char *__doc_amitex_Material_numberCoeffK =
R"doc(Returns:
    the number of diffusion coefficient)doc";

static const char *__doc_amitex_Material_numberIntVars =
R"doc(Returns:
    number an internal variables)doc";

static const char *__doc_amitex_Material_numberZones =
R"doc(Returns:
    the number of zones)doc";

static const char *__doc_amitex_Material_setCoeff =
R"doc(Set mechanics coefficient constant value

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficient)doc";

static const char *__doc_amitex_Material_setCoeffComposite =
R"doc(Set mechanics coefficient constant value (for composite materials)

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficient)doc";

static const char *__doc_amitex_Material_setCoeffCompositeName =
R"doc(Set name of composite coefficient (optional)

Parameter ``name``:
    name)doc";

static const char *__doc_amitex_Material_setCoeffCompositeZone =
R"doc(Set mechanics coefficient constant zone values (for composite materials)

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficient)doc";

static const char *__doc_amitex_Material_setCoeffCompositeZoneFromBin =
R"doc(Set mechanics coefficient constant zone values (for composite materials)

Parameter ``id``:
    index of coefficient

Parameter ``binPath``:
    path to BIN file)doc";

static const char *__doc_amitex_Material_setCoeffComposites =
R"doc(Set mechanics coefficient constant value (for composite materials)

Parameter ``coefficients``:
    values of coefficients)doc";

static const char *__doc_amitex_Material_setCoeffK =
R"doc(Set diffusion coefficient constant value

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficients)doc";

static const char *__doc_amitex_Material_setCoeffKName =
R"doc(Set name of diffusion coefficient (optional)

Parameter ``name``:
    name)doc";

static const char *__doc_amitex_Material_setCoeffKZone =
R"doc(Set diffusion coefficient constant zone values

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficient)doc";

static const char *__doc_amitex_Material_setCoeffKZoneFromBin =
R"doc(Set diffusion coefficient constant zone values

Parameter ``id``:
    index of coefficient

Parameter ``binPath``:
    path to BIN file)doc";

static const char *__doc_amitex_Material_setCoeffKs =
R"doc(Set all diffusion coefficients

Parameter ``coefficients``:
    values of coefficients)doc";

static const char *__doc_amitex_Material_setCoeffName =
R"doc(Set name of coefficient (optional)

Parameter ``name``:
    name)doc";

static const char *__doc_amitex_Material_setCoeffZone =
R"doc(Set mechanics coefficient constant zone values

Parameter ``id``:
    index of coefficient

Parameter ``coeff``:
    value of coefficient)doc";

static const char *__doc_amitex_Material_setCoeffZoneFromBin =
R"doc(Set mechanics coefficient constant zone values

Parameter ``id``:
    index of coefficient

Parameter ``binPath``:
    path to BIN file)doc";

static const char *__doc_amitex_Material_setCoeffs =
R"doc(Set all mechanics coefficients

Parameter ``coefficients``:
    values of coefficient)doc";

static const char *__doc_amitex_Material_setIndex = R"doc()doc";

static const char *__doc_amitex_Material_setIntVarName =
R"doc(Set name of internal variable

Parameter ``name``:
    name)doc";

static const char *__doc_amitex_Material_setLaw =
R"doc(Set behavior law for mechanics

Parameter ``law``:
    name of law

Parameter ``lib``:
    path to the library implementing the law)doc";

static const char *__doc_amitex_Material_setLawK =
R"doc(Set behavior law for diffusion

Parameter ``law``:
    name of law

Parameter ``lib``:
    path to the library implementing the law)doc";

static const char *__doc_amitex_Material_setNumberCoeff = R"doc(Set the number of mechanics coefficient)doc";

static const char *__doc_amitex_Material_setNumberCoeffComposite = R"doc(Set the number of mechanics coefficient (composite))doc";

static const char *__doc_amitex_Material_setNumberCoeffK = R"doc(Set the number of diffusion coefficient)doc";

static const char *__doc_amitex_Material_setNumberIntVars = R"doc(set number of internal variables)doc";

static const char *__doc_amitex_Material_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Material_xmlTag = R"doc()doc";

static const char *__doc_amitex_Material_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Material_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Material_zones =
R"doc(Returns:
    zone list)doc";

static const char *__doc_amitex_Material_zones_2 = R"doc()doc";

static const char *__doc_amitex_Material_zones_3 = R"doc()doc";

static const char *__doc_amitex_Materials = R"doc(Definition of all materials)doc";

static const char *__doc_amitex_Materials_Materials = R"doc()doc";

static const char *__doc_amitex_Materials_add = R"doc(Add a material)doc";

static const char *__doc_amitex_Materials_add_2 = R"doc()doc";

static const char *__doc_amitex_Materials_add_3 = R"doc(Add a composite material)doc";

static const char *__doc_amitex_Materials_add_4 = R"doc()doc";

static const char *__doc_amitex_Materials_at =
R"doc(Get a material

Parameter ``id``:
    material index)doc";

static const char *__doc_amitex_Materials_at_2 = R"doc()doc";

static const char *__doc_amitex_Materials_begin = R"doc()doc";

static const char *__doc_amitex_Materials_begin_2 = R"doc()doc";

static const char *__doc_amitex_Materials_composite = R"doc()doc";

static const char *__doc_amitex_Materials_composite_2 = R"doc()doc";

static const char *__doc_amitex_Materials_composites = R"doc(Composite materials)doc";

static const char *__doc_amitex_Materials_end = R"doc()doc";

static const char *__doc_amitex_Materials_end_2 = R"doc()doc";

static const char *__doc_amitex_Materials_interphase = R"doc(Interphase materials **note**: In general this element is constructed by Input::generateFiles)doc";

static const char *__doc_amitex_Materials_material =
R"doc(Get a material

Parameter ``id``:
    material index)doc";

static const char *__doc_amitex_Materials_material_2 = R"doc()doc";

static const char *__doc_amitex_Materials_materials = R"doc()doc";

static const char *__doc_amitex_Materials_numberComposites = R"doc(Get the number composite of materials)doc";

static const char *__doc_amitex_Materials_numberMaterials = R"doc(Get the number of materials)doc";

static const char *__doc_amitex_Materials_referenceMaterial = R"doc(reference material (mechanics))doc";

static const char *__doc_amitex_Materials_referenceMaterialD = R"doc(reference material (diffusion))doc";

static const char *__doc_amitex_Materials_setLastId = R"doc()doc";

static const char *__doc_amitex_Materials_setNumberMaterials =
R"doc(Set the number of materials (can substract materials or add empty materials )

Parameter ``num``:
    desired number of materials)doc";

static const char *__doc_amitex_Materials_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Materials_xmlTag = R"doc()doc";

static const char *__doc_amitex_Materials_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Materials_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_MechanicDriving = R"doc(Types of driving for mechanical problems)doc";

static const char *__doc_amitex_MechanicDriving_Strain = R"doc()doc";

static const char *__doc_amitex_MechanicDriving_Stress = R"doc()doc";

static const char *__doc_amitex_Mechanics = R"doc()doc";

static const char *__doc_amitex_Mechanics_C0Sym = R"doc()doc";

static const char *__doc_amitex_Mechanics_Mechanics = R"doc()doc";

static const char *__doc_amitex_Mechanics_Mechanics_2 = R"doc()doc";

static const char *__doc_amitex_Mechanics_filter = R"doc()doc";

static const char *__doc_amitex_Mechanics_smallPerturbations = R"doc()doc";

static const char *__doc_amitex_Mechanics_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Mechanics_xmlTag = R"doc()doc";

static const char *__doc_amitex_Mechanics_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Mechanics_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Output = R"doc(Output parametrization)doc";

static const char *__doc_amitex_Output_VtkIntVarList = R"doc()doc";

static const char *__doc_amitex_Output_VtkIntVarList_VtkIntVarList = R"doc()doc";

static const char *__doc_amitex_Output_VtkIntVarList_numM = R"doc()doc";

static const char *__doc_amitex_Output_VtkIntVarList_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Output_Zone = R"doc()doc";

static const char *__doc_amitex_Output_Zone_Zone = R"doc()doc";

static const char *__doc_amitex_Output_Zone_Zone_2 =
R"doc(Parameter ``numM``:
    material index)doc";

static const char *__doc_amitex_Output_Zone_numM = R"doc()doc";

static const char *__doc_amitex_Output_Zone_setVarIntList =
R"doc(Set the list of internal variables to be printed

Parameter ``list``:
    list of indices of the internal variables)doc";

static const char *__doc_amitex_Output_Zone_varIntList = R"doc()doc";

static const char *__doc_amitex_Output_Zone_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Output_Zone_xmlTag = R"doc()doc";

static const char *__doc_amitex_Output_Zone_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Output_Zone_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Output_addVtkIntVarList = R"doc(Set the internal variables for field output for a given material)doc";

static const char *__doc_amitex_Output_addZone = R"doc(Add zones of a material to .zstd output)doc";

static const char *__doc_amitex_Output_addZone_2 = R"doc()doc";

static const char *__doc_amitex_Output_addZone_3 =
R"doc(Add zones of a material to .zstd output

Parameter ``numM``:
    index of material

Parameter ``intVarList``:
    list of internal variable indices)doc";

static const char *__doc_amitex_Output_intVarList = R"doc()doc";

static const char *__doc_amitex_Output_setVtkFluxDGradD = R"doc(Control output of diffusion flux and gradient)doc";

static const char *__doc_amitex_Output_setVtkStressStrain = R"doc(Control output of stress and strain)doc";

static const char *__doc_amitex_Output_vtkFluxDGradD = R"doc()doc";

static const char *__doc_amitex_Output_vtkStressStrain = R"doc()doc";

static const char *__doc_amitex_Output_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Output_xmlTag = R"doc()doc";

static const char *__doc_amitex_Output_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Output_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Output_zones = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial = R"doc(Reference material for mechanics)doc";

static const char *__doc_amitex_ReferenceMaterialD = R"doc(Reference material for diffusion)doc";

static const char *__doc_amitex_ReferenceMaterialD_K0 = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterialD_ReferenceMaterialD = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterialD_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterialD_xmlTag = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterialD_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterialD_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_ReferenceMaterial = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_lambda0 = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_mu0 = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_xmlTag = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_ReferenceMaterial_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_SmallPerturbations = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_SmallPerturbations_2 = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_SmallPerturbations_3 = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_displacementGradient = R"doc(Toggle non symmetrized displacement gradients (="nsysm" or ommited))doc";

static const char *__doc_amitex_SmallPerturbations_operator_assign = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_operator_bool = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_value = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_xmlTag = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_SmallPerturbations_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Substepping = R"doc(Substepping)doc";

static const char *__doc_amitex_Substepping_Substepping = R"doc()doc";

static const char *__doc_amitex_Substepping_depth = R"doc(maximum number of impricated substeppings)doc";

static const char *__doc_amitex_Substepping_geomRatio = R"doc(after each substepping Nitermax2 = Geom_ratio * Nitermax2)doc";

static const char *__doc_amitex_Substepping_nitermax2 = R"doc(initial number of iterations before supstepping)doc";

static const char *__doc_amitex_Substepping_nsub = R"doc(number of stubsteps when activating a new substepping)doc";

static const char *__doc_amitex_Substepping_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Substepping_xmlTag = R"doc()doc";

static const char *__doc_amitex_Substepping_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Substepping_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Type = R"doc(Helper class to defined elements with <ELEMENT Type="…"> signature)doc";

static const char *__doc_amitex_Type_Type = R"doc()doc";

static const char *__doc_amitex_Type_Type_2 = R"doc()doc";

static const char *__doc_amitex_Type_operator_assign = R"doc()doc";

static const char *__doc_amitex_Type_tag = R"doc()doc";

static const char *__doc_amitex_Type_type = R"doc()doc";

static const char *__doc_amitex_Type_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Type_xmlTag = R"doc()doc";

static const char *__doc_amitex_Type_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Type_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_UserInterruption = R"doc(Define user interruptions)doc";

static const char *__doc_amitex_UserInterruption_UserInterruption = R"doc()doc";

static const char *__doc_amitex_UserInterruption_UserInterruption_2 = R"doc()doc";

static const char *__doc_amitex_UserInterruption_index = R"doc()doc";

static const char *__doc_amitex_UserInterruption_setIndex = R"doc(Set index)doc";

static const char *__doc_amitex_UserInterruption_value = R"doc()doc";

static const char *__doc_amitex_UserInterruption_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_UserInterruption_xmlTag = R"doc()doc";

static const char *__doc_amitex_UserInterruption_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_UserInterruption_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Value = R"doc(Helper class to easily interact with elements of <ELEMENT Value="…"> signature)doc";

static const char *__doc_amitex_Value_Value = R"doc()doc";

static const char *__doc_amitex_Value_Value_2 = R"doc()doc";

static const char *__doc_amitex_Value_Value_3 = R"doc()doc";

static const char *__doc_amitex_Value_operator_T0 = R"doc()doc";

static const char *__doc_amitex_Value_operator_assign = R"doc()doc";

static const char *__doc_amitex_Value_tag = R"doc()doc";

static const char *__doc_amitex_Value_value = R"doc(Get the value)doc";

static const char *__doc_amitex_Value_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_Value_xmlTag = R"doc()doc";

static const char *__doc_amitex_Value_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_Value_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_VoxelSpec = R"doc(Basic specification (index, volume fraction, zone) of a voxel (composite if more than one phase))doc";

static const char *__doc_amitex_VoxelSpec_VoxelSpec = R"doc()doc";

static const char *__doc_amitex_VoxelSpec_VoxelSpec_2 =
R"doc(Parameter ``phases``:
    list of (material index, volume fraction))doc";

static const char *__doc_amitex_VoxelSpec_VoxelSpec_3 =
R"doc(Parameter ``phases``:
    list of (material index, volume fraction, zone index))doc";

static const char *__doc_amitex_VoxelSpec_phases = R"doc((material index, volume fraction, zone))doc";

static const char *__doc_amitex_VtkFluxDGradD = R"doc(Control output of diffusion flux and gradient)doc";

static const char *__doc_amitex_VtkFluxDGradD_VtkFluxDGradD = R"doc()doc";

static const char *__doc_amitex_VtkFluxDGradD_VtkFluxDGradD_2 = R"doc()doc";

static const char *__doc_amitex_VtkFluxDGradD_fluxd = R"doc(Output flux to VTK file(s) (0 or 1))doc";

static const char *__doc_amitex_VtkFluxDGradD_gradd = R"doc(Output gradient to VTK file(s) (0 or 1))doc";

static const char *__doc_amitex_VtkFluxDGradD_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_VtkFluxDGradD_xmlTag = R"doc()doc";

static const char *__doc_amitex_VtkFluxDGradD_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_VtkFluxDGradD_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_VtkHeader = R"doc(Information contained in the header of a VTK file)doc";

static const char *__doc_amitex_VtkHeader_cellDataSize = R"doc(scalar type)doc";

static const char *__doc_amitex_VtkHeader_dimensions = R"doc()doc";

static const char *__doc_amitex_VtkHeader_origin = R"doc(grid dimensions)doc";

static const char *__doc_amitex_VtkHeader_scalarType = R"doc(grid origin)doc";

static const char *__doc_amitex_VtkHeader_spacing = R"doc(grid dimensions)doc";

static const char *__doc_amitex_VtkStressStrain = R"doc(Control output of stress and strain)doc";

static const char *__doc_amitex_VtkStressStrain_VtkStressStrain = R"doc()doc";

static const char *__doc_amitex_VtkStressStrain_VtkStressStrain_2 = R"doc()doc";

static const char *__doc_amitex_VtkStressStrain_strain = R"doc(Output strain to VTK file(s) (0 or 1))doc";

static const char *__doc_amitex_VtkStressStrain_stress = R"doc(Output stress to VTK file(s) (0 or 1))doc";

static const char *__doc_amitex_VtkStressStrain_xmlHasBody = R"doc()doc";

static const char *__doc_amitex_VtkStressStrain_xmlTag = R"doc()doc";

static const char *__doc_amitex_VtkStressStrain_xmlWriteAttributes = R"doc()doc";

static const char *__doc_amitex_VtkStressStrain_xmlWriteInner = R"doc()doc";

static const char *__doc_amitex_Zone = R"doc(Zone, that is a list of voxel positions)doc";

static const char *__doc_amitex_Zone_Zone =
R"doc(Parameter ``position``:
    list of grid coordinates

Parameter ``gridDims``:
    grid dimensions)doc";

static const char *__doc_amitex_Zone_Zone_2 =
R"doc(Parameter ``gridDims``:
    grid dimensions

Parameter ``position``:
    in grid coordinates)doc";

static const char *__doc_amitex_Zone_Zone_3 =
R"doc(Parameter ``gridDims``:
    grid dimensions

Parameter ``begin``:
    iterator on GridPoint

Parameter ``end``:
    iterator on GridPoint)doc";

static const char *__doc_amitex_Zone_add =
R"doc(add a voxel to a zone

Parameter ``position``:
    voxel grid coordinate)doc";

static const char *__doc_amitex_Zone_add_2 =
R"doc(add a voxel to a zone

Parameter ``position``:
    voxel grid linearized position)doc";

static const char *__doc_amitex_Zone_dims = R"doc()doc";

static const char *__doc_amitex_Zone_linearPositions = R"doc(linear index of positions)doc";

static const char *__doc_amitex_Zone_linearPositions_2 = R"doc()doc";

static const char *__doc_amitex_Zone_numberVoxels =
R"doc(Returns:
    number of voxels)doc";

static const char *__doc_amitex_buildMaterials =
R"doc(Build materials from a list of all voxels (ie indexed by their linearized positions)

Parameter ``phases``:
    array as field of voxel specs

Parameter ``ordering``:
    ordering of positions)doc";

static const char *__doc_amitex_buildMaterials_2 =
R"doc(Build materials from a list of all voxels (ie indexed by their linearized positions) with normal information

Parameter ``phases``:
    array as tuple (voxel specs, normal)

Parameter ``ordering``:
    ordering of positions)doc";

static const char *__doc_amitex_buildMaterials_3 = R"doc()doc";

static const char *__doc_amitex_buildMaterials_4 = R"doc(With normals)doc";

static const char *__doc_amitex_buildMaterialsFromVtk =
R"doc(Build materials from VTK files 'material ids' and 'zone ids'

Parameter ``materials``:
    $Parameter ``materialIdPath``:

path to VTK file 'material ids'

Parameter ``zoneIdPath``:
    path to VTK file 'zone ids'

Parameter ``minId``:
    minimum id (typically for AMITEX it is 1))doc";

static const char *__doc_amitex_driverInputHandler =
R"doc(Connected to AMITEX external driving API so that AmitexError are thrown

Throws:
    AmitexError)doc";

static const char *__doc_amitex_getSimulationShellCommand =
R"doc(Returns:
    shell command to run `amitex_fftp` (with mpirun)

Parameter ``input``:
    input used by the simulation

Parameter ``numberProcs``:
    number of MPI processes used)doc";

static const char *__doc_amitex_linearize =
R"doc(Convert to a linearized position

Parameter ``p``:
    position

Parameter ``dims``:
    grid dimensions

Returns:
    linear position (leading dimension is Z, then Y, X))doc";

static const char *__doc_amitex_readBin =
R"doc(Read BIN file

Parameter ``path``:
    path to file

Parameter ``data``:
    data vector (in VTK voxel order)

Returns:
    type of data in filed

Throws:
    std::runtime_error on input error or file not)doc";

static const char *__doc_amitex_readVTK =
R"doc(Read VTK file

Parameter ``path``:
    path to file

Parameter ``data``:
    data vector (in VTK voxel order)

Returns:
    header data

Throws:
    std::runtime_error on input error or file not readable **note**: only defined for (unsigned) int, (unsigned) short, (unsigned) long, (unsigned) long long and double)doc";

static const char *__doc_amitex_runSimulationExternal =
R"doc(Initialize input files and launch simulation by executing `amitex_fftp`

Parameter ``input``:
    input used by the simulation

Parameter ``numberProcs``:
    number of MPI processes used

Throws:
    AmitexError when the simulation does no exit normally

When `numberProcs` is 0, the number of available MPI processes is used (exact behavior depends on the MPI environment))doc";

static const char *__doc_amitex_unnamed_class_at_include_amitex_component_hpp_18_7 = R"doc()doc";

static const char *__doc_amitex_writeBIN = R"doc(Write an AMITEX BIN file)doc";

static const char *__doc_amitex_writeBIN_2 = R"doc()doc";

static const char *__doc_amitex_writeVTK =
R"doc(write a scalar-field data to a legacy VTK file

Parameter ``path``:
    file to write

Parameter ``nx``:
    grid dimensions

Parameter ``dx``:
    lengths of a voxel

Parameter ``data``:
    data buffer

Parameter ``data_size``:
    number of elements)doc";

static const char *__doc_amitex_writeVTK_2 = R"doc()doc";

static const char *__doc_amitex_writeVTK_3 = R"doc()doc";

static const char *__doc_amitex_writeVTK_4 = R"doc()doc";

static const char *__doc_amitex_writeXML = R"doc()doc";

static const char *__doc_amitex_writeXMLAttributes = R"doc()doc";

static const char *__doc_amitex_writeXMLAttributes_2 = R"doc()doc";

static const char *__doc_amitex_writeXMLFile = R"doc()doc";

static const char *__doc_amitex_writeXMLFileStream = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

