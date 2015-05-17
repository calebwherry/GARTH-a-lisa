#ifndef LINE_H
#define LINE_H

#include "boost/compute.hpp"

struct Line 
{
  float m;
  float b;
};

BOOST_COMPUTE_ADAPT_STRUCT(Line, Line, (m, b));

#endif
