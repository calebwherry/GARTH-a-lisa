#include "Canvas.h"

Canvas::Canvas(uint32_t width, uint32_t height)
{
  this->width = width;
  this->height = height;
  this->canvas.resize(width, std::vector<char>(height,0));
}

void Canvas::drawTriangle(float t)
{
}

void Canvas::save(std::string path)
{
}
