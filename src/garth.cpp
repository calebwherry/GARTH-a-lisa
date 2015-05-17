#include "ClDevice.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <unordered_map>
#include <string>

using namespace std;
using Devices = vector<ClDevice>;
using DeviceMap = unordered_map<cl_device_type, Devices>;

DeviceMap getOpenCLDevices();

int main(int argc, char** argv)
{
  DeviceMap deviceMap;
  try 
  {
    deviceMap = getOpenCLDevices();
    for(const auto& pair : deviceMap)
    {
      for(const auto& device : pair.second) 
      {
        cout << "Device: " << device.device_name << " (" << device.device_type << ")" << endl;
      }
    }
  }
  catch (exception& e) 
  {
    cout << "Error: " << e.what() << endl;
  }

  // Get preferred device:
  ClDevice preferred_device;
  auto devicesItr = deviceMap.find(CL_DEVICE_TYPE_GPU);
  if (devicesItr != deviceMap.end())
  {
    preferred_device = devicesItr->second[0];
  }
  else
  {
    devicesItr = deviceMap.find(CL_DEVICE_TYPE_CPU);
    if (devicesItr != deviceMap.end())
    {
      preferred_device = devicesItr->second[0];
    }
    else
    {
      throw runtime_error("Can't find a preferred device");
    }
  }

  cout << "Preferred device: " << preferred_device.device_name << endl;

  return 0;
}

DeviceMap getOpenCLDevices()
{
  DeviceMap deviceMap;

  // Get OpenCL platform ID for NVIDIA if avaiable, otherwise default
  char cBuffer[1024];
  cl_platform_id clSelectedPlatformID = NULL;
  vector<cl_platform_id> clPlatformIDs;

  cl_uint num_platforms;
  cl_int ciErrNum = clGetPlatformIDs(0, NULL, &num_platforms);
  if (ciErrNum != CL_SUCCESS) 
  {
    throw runtime_error("Can't get number of OpenCL platforms");
  }

  if (num_platforms == 0) 
  {
    throw runtime_error("No OpenCL platforms found");
  } 

  // Get platforms:
  clPlatformIDs.resize(num_platforms);
  ciErrNum = clGetPlatformIDs (num_platforms, clPlatformIDs.data(), NULL);
  if (ciErrNum != CL_SUCCESS) 
  {
    throw runtime_error("Can't get platform IDs");
  }

  for(const auto& platform_id : clPlatformIDs) 
  {
    // Get devices:
    cl_uint num_devices;
    vector<cl_device_id> clDevices;
    ciErrNum = clGetDeviceIDs (platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
    if (ciErrNum != CL_SUCCESS)
    {
      throw runtime_error("Can't get number of devices for some platform");
    }

    if (num_devices == 0)
    {
      throw runtime_error(" No devices found supporting OpenCL");
    }

    // Get device ID's:
    clDevices.resize(num_devices);
    ciErrNum = clGetDeviceIDs (platform_id, CL_DEVICE_TYPE_ALL, num_devices, clDevices.data(), NULL);
    if (ciErrNum != CL_SUCCESS) 
    {
      throw runtime_error("Can't get device IDs");
    }

    for(const auto& device_id : clDevices) 
    {
      ClDevice device;
      device.device_id = device_id;
      device.platform_id = platform_id;

      cl_device_type device_type;
      ciErrNum = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
      if (ciErrNum != CL_SUCCESS) 
      {
        throw runtime_error("Can't get device type");
      }
      switch(device_type)
      {
        case CL_DEVICE_TYPE_CPU:
          device.device_type = "CPU";
          break;
        case CL_DEVICE_TYPE_GPU:
          device.device_type = "GPU";
          break;
        case CL_DEVICE_TYPE_ACCELERATOR:
          device.device_type = "ACCELERATOR";
          break;
        case CL_DEVICE_TYPE_DEFAULT:
          device.device_type = "DEFAULT";
          break;
        case CL_DEVICE_TYPE_CUSTOM:
          device.device_type = "CUSTOM";
          break;
        default:
          throw runtime_error("Unknown Device Type");
          break;
      }

      char device_name_buffer[1024];
      ciErrNum = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name_buffer), &device_name_buffer, NULL);
      if (ciErrNum != CL_SUCCESS) 
      {
        throw runtime_error("Can't get device name");
      }
      device.device_name = string(device_name_buffer);
      
      if (deviceMap.find(device_type) == deviceMap.end())
      {
        Devices device_ids;
        device_ids.push_back(device);

        deviceMap.insert(make_pair<cl_device_type&, Devices&>(device_type, device_ids));
      }
      else
      {
        deviceMap[device_type].push_back(device);
      }
    }
  }

  return deviceMap;
}
