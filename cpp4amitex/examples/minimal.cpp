#include "amitex/input.hpp"
#include "amitex/simulation.hpp"

int main() {
  // 32x32x32 grid of voxels of size 1.
  amitex::Grid grid{{32, 32, 32}, {1., 1., 1.}};

  // Set algorithm parameters
  auto algorithm = amitex::Algorithm::create("Basic_Scheme", true);
  // Define a mechanical resolution
  auto mechanics = amitex::Mechanics::create("Default", true);

  auto algorithmParameters = amitex::AlgorithmParameters::create(algorithm, mechanics);

  // Define one elastic material
  auto material = amitex::Material::create();
  material->setLaw("elasiso");
  // Lamé coefficients
  material->setCoeffs({1.e9, 1.5e9});
  // Define the material on all voxels of the unit cell
  auto zone = amitex::Zone::create(grid.dims());
  for (auto p : grid.allPoints()) zone->add(p);
  material->addZone(zone);
  auto materials = amitex::Materials::create();
  materials->add(material);
  // Lamé coefficients of the reference material
  materials->referenceMaterial = amitex::ReferenceMaterial::create(1.e9, 1.5e9);

  auto loadingOutput = amitex::LoadingOutput::create();

  auto output = amitex::Output::create();
  output->setVtkStressStrain(1, 1);
  loadingOutput->output = output;

  // Define one loading
  auto loading = amitex::Loading::create();
  // time between 0 and 1., discretized in 2 steps;
  loading->setTimeDiscretizationLinear(2, 1.);
  // Impose a strain along the zz plane, evolving to 0.01 until the final time
  loading->setEvolution(amitex::Component::ZZ, amitex::MechanicDriving::Strain,
                        amitex::Evolution::Linear, 0.01);
  // Relax other directions
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      if (i != amitex::Component::Z || j != amitex::Component::Z)
        loading->setEvolution({i, j}, amitex::MechanicDriving::Stress, amitex::Evolution::Linear,
                              0.00);
    }
  }
  loadingOutput->add(loading);

  // Define all input categories
  auto input = amitex::Input::create(grid, algorithmParameters, materials, loadingOutput);

  // Define output.std in "amitex_dir"
  input->resultsDir = "amitex_dir";

  // Run amitex_fftp on two processes
  amitex::runSimulationExternal(*input, 2);
}
