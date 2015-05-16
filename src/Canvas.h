#include <string>
#include <vector>

class Canvas {
  public:
    Canvas()=delete;
    Canvas(uint32_t width, uint32_t height);
    void drawTriangle(float t);
    void save(std::string path);

  private:
    uint32_t width;
    uint32_t height;
    std::vector<std::vector<char>> canvas;
};
