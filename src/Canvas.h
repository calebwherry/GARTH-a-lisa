#include <string>
#include <vector>

#include "Color.h"
#include "Triangle.h"

class Canvas {
  public:
    Canvas()=delete;
    Canvas(uint32_t width, uint32_t height);
    void drawTriangle(const Triangle& t);
    void save(const std::string& path);

  private:
    uint32_t width;
    uint32_t height;
    std::vector<std::vector<Color>> canvas;
};
