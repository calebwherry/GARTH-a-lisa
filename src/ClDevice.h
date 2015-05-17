#ifndef CLDEVICE_H
#define CLDEVICE_H

#include <CL/cl.h>
#include <string>

struct ClDevice
{
  cl_platform_id platform_id;
  cl_device_id device_id;
  std::string device_name;
  std::string device_type;
};

#endif
