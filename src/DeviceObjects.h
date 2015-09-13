#ifndef DEVICE_OBJECTS_H
#define DEVICE_OBJECTS_H

#include "boost/compute.hpp"

namespace bc = boost::compute;

struct DeviceObjects
{
  bc::kernel kernel;
  bc::buffer triangles;
  bc::buffer canvas;
  bc::context context;
  bc::command_queue queue;
  bc::device device;
};

#endif
