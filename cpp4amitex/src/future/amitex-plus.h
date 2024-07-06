#ifndef _AMITEX_CAPIPLUS_HEADER_
#define _AMITEX_CAPIPLUS_HEADER_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "amitex.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t nx[3];
  double dx[3];
  double x0[3];
  bool pbc[3];
  size_t p_row, p_col;
} InputGrid;

typedef struct {
  char* data;
  size_t len;
} PCharLenPair;

typedef struct {
  PCharLenPair log, output_prefix, algo, materials, loading;
} InputFiles;

void amitex_get_pencil_decomposition(size_t istart[3], size_t iend[3]);
void amitex_initialize(int isimu);
void amitex_initialize_stage_one(const InputGrid* grid, const InputFiles* ifiles);
void amitex_initialize_stage_two(int32_t* numM, int64_t* numZ, const InputFiles* ifiles);
void amitex_loading_set_diffusion_driving(int index_varD, int direction, double value, int driving);
void amitex_loading_set_mechanics_driving(int direction, double value, int driving);
void amitex_extract_average_deformation(double* def);
void amitex_extract_average_stress(double* stress);
void amitex_extract_average_diffusion_gradient(int indexD, double* gradQD);
void amitex_extract_average_diffusion_flux(int indexD, double* fluxD);
size_t amitex_get_tensor_size();
void amitex_material_set_coeff_by_zone(int numM, int numCoeff, const double* values,
                                       size_t numberValues);
void amitex_material_set_coeffk_by_zone(int numM, int numCoeff, const double* values,
                                        size_t numberValues);
void amitex_get_voxel_partition_from_grid(const InputGrid* grid, size_t istart[3], size_t iend[3]);
#ifdef __cplusplus
}
#endif

#endif  // _AMITEX_CAPIPLUS_HEADER_
