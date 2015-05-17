#include "boost/compute.hpp"
#include "draw_triangle.h"

#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <unordered_map>
#include <string>

using namespace std;
namespace bc = boost::compute;

bc::kernel make_kernel(const bc::context& ctx, const char* source, std::string name);

int main(int argc, char** argv)
{
  bc::device device = bc::system::default_device();
  bc::context context(device);
  bc::command_queue queue(context, device);
  bc::kernel kernel = make_kernel(context, garth_kernels::draw_triangle, "draw_triangle");

  cout << "Preferred device: " << device.name() << endl;

  // setup input arrays
  float a[] = { 10, 0, 30, -4 };
  float b[] = { 5, 0, 7, 8 };

  // make space for the output
  float c[] = { 0, 10, 0, 0 };

  // create memory buffers for the input and output
  bc::buffer buffer_a(context, 4 * sizeof(float));
  bc::buffer buffer_b(context, 4 * sizeof(float));
  bc::buffer buffer_c(context, 4 * sizeof(float));

  // set the kernel arguments
  kernel.set_arg(0, buffer_a);
  kernel.set_arg(1, buffer_b);
  kernel.set_arg(2, buffer_c);

  // write the data from 'a' and 'b' to the device
  queue.enqueue_write_buffer(buffer_a, 0, 4 * sizeof(float), a);
  queue.enqueue_write_buffer(buffer_b, 0, 4 * sizeof(float), b);

  // run the add kernel
  queue.enqueue_1d_range_kernel(kernel, 0, 4, 0).wait();


  // transfer results back to the host array 'c'
  queue.enqueue_read_buffer(buffer_c, 0, 4 * sizeof(float), c);

  queue.finish();

  // print out results in 'c'
  std::cout << "c: [" << c[0] << ", "
                      << c[1] << ", "
                      << c[2] << ", "
                      << c[3] << "]" << std::endl;

  return 0;
}

bc::kernel make_kernel(const bc::context& context, const char* source, std::string name)
{

    // setup compilation flags for the program
    std::stringstream options;

    // create and build the program
    bc::program program =
        bc::program::build_with_source(source, context, options.str());

    // create and return the kernel
    return program.create_kernel(name);
}
