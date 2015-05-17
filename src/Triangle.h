#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "boost/compute.hpp"
#include "Point.h"
#include "Color.h"

struct Triangle 
{
  Point p;
  Point q;
  Point r;

  Color c;
};

BOOST_COMPUTE_ADAPT_STRUCT(Triangle, Triangle, (p, q, r, c));

#endif
