#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(Color c, ThreePoints<uint32_t> verts) : color(c), vertices(verts)
{
}

Color const& Triangle::getColor()
{
  return color;
}

void Triangle::setColor (Color c)
{
  color = c;
}

ThreePoints<uint32_t> const& Triangle::getVertices()
{
  return vertices;
}

void Triangle::setVertices(ThreePoints<uint32_t> verts)
{
  vertices = verts;
}
