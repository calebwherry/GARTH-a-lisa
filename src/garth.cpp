#define BOOST_COMPUTE_DEBUG_KERNEL_COMPILATION
#include "boost/compute.hpp"

#include "Canvas.h"
#include "Triangle.h"
#include "draw_triangle.h"

#include <vector>
#include <iostream>
#include <string>
#include <exception>

using namespace std;
namespace bc = boost::compute;

bc::kernel make_kernel(const bc::context& ctx, const char* source, std::string name);

int main(int argc, char** argv)
{
  bc::device device = bc::system::default_device();
  bc::context context(device);
  bc::command_queue queue(context, device);
  bc::kernel kernel;

  try
  {
    kernel = make_kernel(context, garth_kernels::draw_triangles.c_str(), "draw_triangles");
  }
  catch (bc::opencl_error& ex)
  {
    cout << "Boost.Compute error: " << ex.error_string() << endl;
    cout << "Boost.Compute error_code: " << ex.error_code() << endl;
    return 1;
  }
  catch (exception& ex)
  {
    cout << "WTF HAPPENED?!?!~?! - " << ex.what() << endl;
    return 1;
  }

  cout << "Default device: " << device.name() << endl;

  // Settings:
  uint32_t num_triangles = 10,
           canvas_width = 100,
           canvas_height = 100;

  // Create RNG
  std::default_random_engine rng;
  std::uniform_int_distribution<uint8_t> random_color(0,255);
  
  // Create Triangle buffer:
  vector<Triangle> triangles(num_triangles);
  for(auto& t : triangles)
  {
    t.c.Red = random_color(rng);
    t.c.Green = random_color(rng);
    t.c.Blue = random_color(rng);
  }
  Canvas canvas(canvas_width, canvas_height);

  // Create memory buffers for the input and output:
  bc::buffer buffer_triangles(context, sizeof(triangles));
  bc::buffer buffer_canvas(context, sizeof(canvas));

  // Set the kernel arguments:
  kernel.set_arg(0, buffer_triangles);
  kernel.set_arg(1, num_triangles);
  kernel.set_arg(2, buffer_canvas);
  kernel.set_arg(3, canvas_width);
  kernel.set_arg(4, canvas_height);
  

  // Write the data to the device:
  queue.enqueue_write_buffer(buffer_triangles, 0, sizeof(triangles.data()), triangles.data());
  queue.enqueue_write_buffer(buffer_canvas, 0, sizeof(canvas.getCanvas().data()), canvas.getCanvas().data());

  // Calculate local/global group sizes:
  bc::extents<2> offsetRange(0);
  bc::extents<2> globalRange;
  globalRange[0] = 10;
  globalRange[1] = 10;

  bc::extents<2> localRange;
  localRange[0] = 10;
  localRange[1] = 10;

  // Run kernel:
  queue.enqueue_nd_range_kernel(kernel, offsetRange, globalRange, localRange);

  // transfer results back to the host
  queue.enqueue_read_buffer(buffer_canvas, 0, sizeof(canvas.getCanvas().data()), canvas.getCanvas().data());

  canvas.save("triangles.png");
  return 0;
}

bc::kernel make_kernel(const bc::context& context, const char* source, std::string name)
{
    // setup compilation flags for the program
    std::string options;

    // create and build the program
    bc::program program =
        bc::program::build_with_source(source, context, options.c_str());

    // create and return the kernel
    return program.create_kernel(name);
}
