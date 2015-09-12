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
      __kernel void draw_triangles(
        __global const Triangle *triangles,
        const uint num_triangles,
        __global Color *canvas,
        const uint canvas_width,
        const uint canvas_height)
      {

        const uint i = get_global_id(0);
        const uint j = get_global_id(1);
        if ( (i < canvas_width) && (j < canvas_height) ) 
        {
          uint triangleCount;
          for (triangleCount = 0; triangleCount < num_triangles; ++triangleCount)
          {
            Triangle t = triangles[triangleCount];
            Point centroid;
            centroid.x = (t.p.x + t.q.x + t.r.x) / 3;
            centroid.y = (t.p.y + t.q.y + t.r.y) / 3;
            bool centroid_above_pq = false;  
            bool centroid_above_qr = false;
            bool centroid_above_rp = false;
            bool pixel_above_pq = false;  
            bool pixel_above_qr = false;
            bool pixel_above_rp = false;


            Line pq;
            if (t.p.x == t.q.x) {
              pq.m = (t.p.y - t.q.y) / 0.0001;
            } else {
              pq.m = (t.p.y - t.q.y) / (t.p.x - t.q.x);
            }
            if (centroid.y > (centroid.x * pq.m + pq.b)) {
              centroid_above_pq = true;  
            }

            Line qr;
            if (t.q.x == t.r.x) {
              qr.m = (t.q.y - t.r.y) / 0.0001;
            } else {
              qr.m = (t.q.y - t.r.y) / (t.q.x - t.r.x);
            }
            if (centroid.y > (centroid.x * qr.m + qr.b)) {
              centroid_above_qr = true;
            }

            Line rp;
            if (t.r.x == t.p.x) {
              rp.m = (t.r.y - t.p.y) / 0.0001;
            } else {
              rp.m = (t.r.y - t.p.y) / (t.r.x - t.p.x);
            }
            if (centroid.y > (centroid.x * rp.m + rp.b)) {
              centroid_above_rp = true;
            }

            // Check pixel:
            if (j > (i * rp.m + rp.b)) {
              pixel_above_rp = true;
            }
            if (j > (i * rp.m + rp.b)) {
              pixel_above_rp = true;
            }
            if (j > (i * rp.m + rp.b)) {
              pixel_above_rp = true;
            }

            // Check pixel against centroid:
            if (centroid_above_pq == pixel_above_pq &&
                centroid_above_qr == pixel_above_qr &&
                centroid_above_rp == pixel_above_rp
            )
            {
              float alpha = t.c.Alpha / 255.0;
              Color c = canvas[i*canvas_width + j];
              c.Red = alpha * t.c.Red + (1 - alpha)*c.Red;
              c.Green = alpha * t.c.Green + (1 - alpha)*c.Green;
              c.Blue = alpha * t.c.Blue + (1 - alpha)*c.Blue;
              canvas[i*canvas_width + j] = c;
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
