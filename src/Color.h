#ifndef COLOR_H
#define COLOR_H

#include "boost/compute.hpp"

struct Color
{
  uint8_t Red   = 0;
  uint8_t Green = 0;
  uint8_t Blue  = 0;
  uint8_t Alpha = 255;
};

BOOST_COMPUTE_ADAPT_STRUCT(Color, Color, (Red, Green, Blue, Alpha));

#endif
