#include <array>
#include <utility>

#include "Color.h"

template <class T>
using ThreePoints = std::array<std::pair<T, T>, 3>;

class Triangle 
{
  public:
    Triangle();

    Triangle(
      Color c,
      ThreePoints<uint32_t> verts
    );

    Color const& getColor();
    void setColor(Color c);

    ThreePoints<uint32_t> const& getVertices();
    void setVertices(ThreePoints<uint32_t> verts);

  private:
    Color color;
    ThreePoints<uint32_t> vertices;
};
