#ifndef POINT_H
#define POINT_H

#include "boost/compute.hpp"

#include <stdint.h>

struct Point
{
  int32_t x = 0;
  int32_t y = 0;
};

BOOST_COMPUTE_ADAPT_STRUCT(Point, Point, (x,y));

#endif
