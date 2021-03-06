#include <string>
#include <vector>

#include "Color.h"
#include "Triangle.h"

class Canvas 
{
  public:
    Canvas()=delete;
    Canvas(uint32_t width, uint32_t height);

    void save(const std::string& file_name);
    std::vector<Color>& getCanvas();

  private:
    uint32_t width;
    uint32_t height;
    std::vector<Color> canvas;
};
