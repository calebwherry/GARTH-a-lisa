#ifndef CANVAS_H
#define CANVAS_H

#include "Color.h"
#include "Triangle.h"

#include <string>
#include <vector>

class Canvas 
{
  public:
    Canvas()=delete;
    Canvas(uint32_t width, uint32_t height);

    static Canvas open(const std::string& fileName);
    void save(const std::string& fileName);
    std::vector<Color>& getCanvas();
    uint32_t getWidth();
    uint32_t getHeight();

  private:
    uint32_t width;
    uint32_t height;
    std::vector<Color> canvas;
};

#endif
