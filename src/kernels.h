#include "boost/compute.hpp"

#include "Point.h"
#include "Color.h"
#include "Triangle.h"
#include "Line.h"

#include <string>

namespace bc = boost::compute;

namespace garth_kernels
{

  // Add custom types:
  std::string custom_types = bc::type_definition<Point>() + "\n" + 
                             bc::type_definition<Color>() + "\n" +
                             bc::type_definition<Triangle>() + "\n" +
                             bc::type_definition<Line>() + "\n";

  // draw_triangles kernel:
  std::string draw_triangles = 
    custom_types +
    BOOST_COMPUTE_STRINGIZE_SOURCE(

      int signFunc (Point p1, Point p2, Point p3)
      {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
      }

      __kernel void draw_triangles(
        __global const Triangle *triangles,
        const uint triangle_id,
        __global Color *canvas,
        const uint canvas_width,
        const uint canvas_height)
      {

        const uint i = get_global_id(0);
        const uint j = get_global_id(1);
        if ( (i < canvas_width) && (j < canvas_height) ) 
        {
          uint triangleCount = 0;
          //for (triangleCount = 0; triangleCount < num_triangles; ++triangleCount)
          {
            Triangle t = triangles[triangle_id];
            Point p = {i,j};
            bool b1 = signFunc(p, t.p, t.q) < 0;
            bool b2 = signFunc(p, t.q, t.r) < 0;
            bool b3 = signFunc(p, t.r, t.p) < 0;

            if ((b1 == b2) && (b2 == b3))
            {
              float alpha = t.c.Alpha / 255.0;
              Color c = canvas[i*canvas_width + j];
              c.Red = alpha * t.c.Red + (1 - alpha)*c.Red;
              c.Green = alpha * t.c.Green + (1 - alpha)*c.Green;
              c.Blue = alpha * t.c.Blue + (1 - alpha)*c.Blue;
              canvas[j*canvas_width + i] = c;
            }
          }
        }
      }

    );


  // Color pixel kernel:
  std::string draw_gradient = 
    custom_types +
    BOOST_COMPUTE_STRINGIZE_SOURCE(
      __kernel void draw_gradient(
        __global Color *canvas,
        const uint canvas_width,
        const uint canvas_height)
      {
        const uint i = get_global_id(0);
        const uint j = get_global_id(1);

        if ( (i < canvas_width) && (j < canvas_height) ) 
        {
          Color c;
          c.Red = i;
          c.Green = 0;
          c.Blue = j;
          canvas[i*canvas_width + j] = c;
        }
      }
    );
}
