#define BOOST_COMPUTE_DEBUG_KERNEL_COMPILATION
#include "boost/compute.hpp"

#include "SimulationParameters.h"
#include "DeviceObjects.h"
#include "Organism.h"
#include "Canvas.h"
#include "Triangle.h"
#include "kernels.h"
#include "lodepng.h"

#include <vector>
#include <iostream>
#include <string>
#include <exception>

std::random_device GLOBAL_RD;
std::default_random_engine GLOBAL_RNG;//(GLOBAL_RD());

using namespace std;
namespace bc = boost::compute;

bool setupContext(DeviceObjects& deviceObjs, const SimulationParameters& params);
bc::kernel compileKernel(const bc::context& ctx, const char* source, std::string name);
void drawTriangles(DeviceObjects& deviceObjects, const SimulationParameters& simParmas, const vector<Triangle>& triangls, const string& imageFileName);
vector<Triangle> createRandomTriangles(const SimulationParameters& params);
Organism simulateSingleGeneration(const Organism& parentOrganism, DeviceObjects& deviceObjects, const SimulationParameters& simParams);

int main(int argc, char** argv)
{
  // Read in reference image:
  string referenceImageFileName = "test.png";
  Canvas referenceImage = Canvas::open(referenceImageFileName);

  // Display all available devices:
  cout << "Available devices:" << endl;
  for (const auto& availableDevice : bc::system::devices())
  {
    cout << availableDevice.name() << " (" << availableDevice.vendor() << ")" << endl;
  }

  // Use default device for now:
  DeviceObjects deviceObjs;
  deviceObjs.device = bc::system::default_device();
  deviceObjs.context = bc::context(deviceObjs.device);
  deviceObjs.queue = bc::command_queue(deviceObjs.context, deviceObjs.device);
  cout << endl << "Default device: " << deviceObjs.device.name() << endl;

  // Move to CLI options:
  SimulationParameters simParams;
  simParams.numTriangles = 100;
  simParams.canvasWidth = referenceImage.getWidth();
  simParams.canvasHeight = referenceImage.getHeight();
  simParams.maxGenerations = 5;

  // Setup context:
  setupContext(deviceObjs, simParams);

  // Setup simulation -- Our best organism at this point is the first one we generate
  Organism bestOrganism;
  bestOrganism.genome = createRandomTriangles(simParams);
  bestOrganism.fitness = 0.0;
  
  // Simulation loop:
  for(uint32_t generationID = 1; generationID <= simParams.maxGenerations; generationID++)
  {
    // 1. Create children from bestOrganism
    // 2. Mate/Mutate children
    // 3. Calculate fitneess
    // 4. Rank organisms
    // 4. Pick best organism
    bestOrganism = simulateSingleGeneration(bestOrganism, deviceObjs, simParams);
    cout << "Generation[" << generationID << "] --> Current Best Organism[" << bestOrganism.id << "] has fitness: " << bestOrganism.fitness << endl;
  }
  
  return 0;
}

Organism simulateSingleGeneration(const Organism& parentOrganism, DeviceObjects& deviceObjects, const SimulationParameters& simParams)
{
  Organism bestOrganism;
  bestOrganism.genome = createRandomTriangles(simParams);
  bestOrganism.fitness = bestOrganism.id * 3.14;
  //writeBlankCanvas(context, queue);

  string bestOrganismImageFileName = "triangles_" + to_string(bestOrganism.id) + ".png";
  drawTriangles(deviceObjects, simParams, bestOrganism.genome, bestOrganismImageFileName);

  return bestOrganism;
}

bc::kernel compileKernel(const bc::context& context, const char* source, std::string name)
{
    // setup compilation flags for the program
    std::string options;

    // create and build the program
    bc::program program =
        bc::program::build_with_source(source, context, options.c_str());

    // create and return the kernel
    return program.create_kernel(name);
}


// Setup context:
bool setupContext(DeviceObjects& deviceObjects, const SimulationParameters& params)
{
  try
  {
    deviceObjects.kernel = compileKernel(deviceObjects.context, garth_kernels::draw_triangles.c_str(), "draw_triangles");
  }
  catch (bc::opencl_error& ex)
  {
    cerr << "Boost.Compute error: '" << ex.error_string() << "'" << endl;
    cerr << "Boost.Compute error_code: '" << ex.error_code() << "'" << endl;
    return false;
  }
  catch (exception& ex)
  {
    cerr << "Non-OpenCL exception encountered: '" << ex.what() << "'" << endl;
    return false;
  }

  // Create memory buffers:
  deviceObjects.triangles = bc::buffer(deviceObjects.context, params.numTriangles * sizeof(Triangle));
  deviceObjects.canvas = bc::buffer(deviceObjects.context, params.canvasWidth * params.canvasHeight * sizeof(Color));

  // Context setup complete:
  return true;
}


// Draw triangles:
void drawTriangles(
  DeviceObjects& deviceObjects, 
  const SimulationParameters& simParams, 
  const vector<Triangle>& triangles,
  const string& imageFileName)
{
  Canvas canvas(simParams.canvasWidth, simParams.canvasHeight);

  // Set the kernel arguments:
  deviceObjects.kernel.set_arg(0, deviceObjects.triangles);
  deviceObjects.kernel.set_arg(2, deviceObjects.canvas);
  deviceObjects.kernel.set_arg(3, simParams.canvasWidth);
  deviceObjects.kernel.set_arg(4, simParams.canvasHeight);  

  // Write the data to the device:
  deviceObjects.queue.enqueue_write_buffer(deviceObjects.triangles, 0, triangles.size() * sizeof(Triangle), triangles.data());
  deviceObjects.queue.enqueue_write_buffer(deviceObjects.canvas, 0, canvas.getCanvas().size() * sizeof(Color), canvas.getCanvas().data());

  // Calculate local/global group sizes:
  bc::extents<2> offsetRange(0);
  bc::extents<2> globalRange;
  globalRange[0] = simParams.canvasWidth;
  globalRange[1] = simParams.canvasHeight;

  bc::extents<2> localRange;
  localRange[0] = 10;
  localRange[1] = 10;

  // Run kernel:
  for(uint32_t i = 0; i < triangles.size(); i++)
  {
    deviceObjects.kernel.set_arg(1, i);
    deviceObjects.queue.enqueue_nd_range_kernel(deviceObjects.kernel, offsetRange, globalRange, localRange);
  }

  // transfer results back to the host
  deviceObjects.queue.enqueue_read_buffer(deviceObjects.canvas, 0, canvas.getCanvas().size() * sizeof(Color), canvas.getCanvas().data());

  // Wait for queued tasks to finish
  deviceObjects.queue.finish();

  // Write image:
  canvas.save(imageFileName);
}

// Create random triangles:
vector<Triangle> createRandomTriangles(
  const SimulationParameters& simParams)
{
  // Create distributions:
  std::uniform_int_distribution<uint8_t> randomColor(0, 255);
  std::uniform_int_distribution<int32_t> randomXPosition(-(int32_t)simParams.canvasWidth*0.1, (int32_t)simParams.canvasWidth*1.1);
  std::uniform_int_distribution<int32_t> randomYPosition(-(int32_t)simParams.canvasHeight*0.1, (int32_t)simParams.canvasHeight*1.1);
  
  // Create Triangle buffer:
  vector<Triangle> triangles(simParams.numTriangles);
  for(auto& t : triangles)
  {
    t.c.Red = randomColor(GLOBAL_RNG);
    t.c.Green = randomColor(GLOBAL_RNG);
    t.c.Blue = randomColor(GLOBAL_RNG);
    t.c.Alpha = randomColor(GLOBAL_RNG);
    t.p.x = randomXPosition(GLOBAL_RNG);
    t.p.y = randomYPosition(GLOBAL_RNG);
    t.q.x = randomXPosition(GLOBAL_RNG);
    t.q.y = randomYPosition(GLOBAL_RNG);
    t.r.x = randomXPosition(GLOBAL_RNG);
    t.r.y = randomYPosition(GLOBAL_RNG);
  }

  return triangles;
}

