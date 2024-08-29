#include "amitex/input/loading.hpp"

#include <algorithm>

namespace amitex {

const char* AxisStrings[3] = {"x", "y", "z"};

void Loading::setParamEvolution(size_t index, Evolution evolution, double value) {
  if (index >= params.size()) params.resize(index + 1);
  params[index].evolution = evolution;
  params[index].value = value;
  params[index].index = index;
}

void Loading::setTemperatureEvolution(Evolution evolution, double value) {
  if (!temperature) temperature = Temperature{};
  temperature.value().evolution = evolution;
  temperature.value().value = value;
}

Loading::GradGradU::GradGradU(AxisIndexPair ii, int j) : tcomponent{ii}, vcomponent{j} {
  tag_ = "gradgradU_";
  tag_ += AxisStrings[ii.first];
  tag_ += AxisStrings[ii.second];
  tag_ += "_";
  tag_ += AxisStrings[static_cast<int>(j)];
}

void Loading::setGradGradU(AxisIndexPair ij, int k, Evolution evolution, double value) {
  int idx = -1;
  for (size_t i = 0; i < gradGradUDrivers.size(); i++) {
    if (gradGradUDrivers[i].tcomponent == ij && gradGradUDrivers[i].vcomponent == k) {
      idx = i;
      break;
    }
  }
  if (idx == -1) {
    GradGradU driver{ij, k};
    gradGradUDrivers.push_back(driver);
    idx = gradGradUDrivers.size() - 1;
  }
  auto& driver = gradGradUDrivers[idx];
  driver.evolution = evolution;
  driver.value = value;
}

}  // namespace amitex