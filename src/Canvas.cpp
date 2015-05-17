#include "Canvas.h"
#include "lodepng.h"

Canvas::Canvas(uint32_t width, uint32_t height)
{
  this->width = width;
  this->height = height;

  Color c; // empty color
  this->canvas.resize(width, std::vector<Color>(height,c));
}

void Canvas::drawTriangle(const Triangle& t)
{
  
}

void Canvas::save(const std::string& path)
{
}
