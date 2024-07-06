#ifndef _MEROPE_AMITEX_MAKE_MATERIALS_HEADER_
#define _MEROPE_AMITEX_MAKE_MATERIALS_HEADER_

#include <string>
#include <utility>

#include "Voxellation/Voxellation.hxx"
#include "amitex/material.hpp"

amitex::Material makeMaterialDiffusion(const merope::vox::Voxellation<3>& voxelation,
                                       const std::pair<std::string, std::string>& law);

//! Define composite materials **and** place pure materials
void makeCompositeMaterials(amitex::Materials& materials, merope::vox::Voxellation<3>& voxelation,
                            const std::string& law);

amitex::AlgorithmParameters defaultDiffusionAlgorithm();
amitex::AlgorithmParameters defaultMechanicsAlgorithm();

#endif  // _MEROPE_AMITEX_MAKE_MATERIALS_HEADER_
