#include "amitex/input/loading.hpp"

#include <algorithm>

namespace amitex {

const char* AxisStrings[3] = {"x", "y", "z"};

static void writeEvolutionAttr(std::ostream& stream, Evolution evolution, double value) {
  switch (evolution) {
    case Evolution::Constant:
      writeXMLAttributes(stream, "Evolution", "Constant");
      break;
    case Evolution::Linear:
      writeXMLAttributes(stream, "Evolution", "Linear");
      writeXMLAttributes(stream, "Value", value);
  }
}

void Loading::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Tag", id + 1);
}

void Loading::xmlWriteInner(std::ostream& stream) const {
  writeXML(stream, tdisc);
  if (userTimeList) writeXML(stream, tlist);
  if (outputVtkList.numberValues() > 0) writeXML(stream, outputVtkList);
  for (const auto& driver : diffu_drivers) {
    writeXML(stream, driver);
  }
  for (const auto& driver : meca_drivers) {
    writeXML(stream, driver);
  }
  if (any_of(meca_drivers.begin(), meca_drivers.end(), [](const auto& d) { return d.dirstress; })) {
    writeXML(stream, dirstress_);
  }
  if (temperature) writeXML(stream, temperature.value());
  for (const auto& param : params) writeXML(stream, param);
  if (outputCell.number >= 0) writeXML(stream, outputCell);
  if (outputZone.number >= 0) writeXML(stream, outputZone);
  for (const auto& driver : gradGradUDrivers) {
    writeXML(stream, driver);
  }
  for (const auto& userItpt : userItpts) writeXML(stream, userItpt);
}

void Loading::MechanicsDriver::xmlWriteAttributes(std::ostream& stream) const {
  switch (driving) {
    case MechanicDriving::Strain:
      writeXMLAttributes(stream, "Driving", "Strain");
      break;
    case MechanicDriving::Stress:

      writeXMLAttributes(stream, "Driving", "Stress");
  }
  switch (evolution) {
    case Evolution::Constant:
      // Redondant (?) if dirstress is imposed
      if (!(driving == MechanicDriving::Stress && dirstress))
        writeXMLAttributes(stream, "Evolution", "Constant");
      break;
    case Evolution::Linear:
      writeXMLAttributes(stream, "Evolution", "Linear");
      writeXMLAttributes(stream, "Value", value);
  }
  if (dirstress) writeXMLAttributes(stream, "DirStress", dirstress.value());
}

void Loading::DiffusionDriver::xmlWriteAttributes(std::ostream& stream) const {
  switch (driving) {
    case DiffusionDriving::Gradient:
      writeXMLAttributes(stream, "Driving", "GradD");
      break;
    case DiffusionDriving::Flux:
      writeXMLAttributes(stream, "Driving", "FluxD");
  }
  switch (evolution) {
    case Evolution::Constant:
      writeXMLAttributes(stream, "Evolution", "Constant");
      break;
    case Evolution::Linear:
      writeXMLAttributes(stream, "Evolution", "Linear");
      writeXMLAttributes(stream, "Value", value);
  }
}

void Loading::DirStress_::xmlWriteAttributes(std::ostream& stream) const {
  switch (dirstress) {
    case Cauchy:
      writeXMLAttributes(stream, "Type", "Cauchy");
      break;
    case PK1:
      writeXMLAttributes(stream, "Type", "PK1");
      break;
  }
}

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

void Loading::Temperature::xmlWriteAttributes(std::ostream& stream) const {
  writeEvolutionAttr(stream, evolution, value);
}

void Loading::Param::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Index", index + 1);
  writeEvolutionAttr(stream, evolution, value);
}

Loading::GradGradU::GradGradU(AxisIndexPair ii, int j)
    : tcomponent{ii}, vcomponent{j}, tag_{"gradgradU_"} {
  tag_ += AxisStrings[ii.first];
  tag_ += AxisStrings[ii.second];
  tag_ += "_";
  tag_ += AxisStrings[static_cast<int>(j)];
}

void Loading::GradGradU::xmlWriteAttributes(std::ostream& stream) const {
  switch (evolution) {
    case Evolution::Constant:
      writeXMLAttributes(stream, "Evolution", "Constant");
      break;
    case Evolution::Linear:
      writeXMLAttributes(stream, "Evolution", "Linear");
      writeXMLAttributes(stream, "Value", value);
  }
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

void Loading::TimeDiscretisation::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Discretization", discretization);
  writeXMLAttributes(stream, "Nincr", nincr);
  if (tfinal > 0.0) {
    writeXMLAttributes(stream, "Tfinal", tfinal);
  }
}

void Loading::OutputNumber::xmlWriteAttributes(std::ostream& stream) const {
  writeXMLAttributes(stream, "Number", number);
}

}  // namespace amitex