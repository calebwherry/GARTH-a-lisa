#include "Canvas.h"
#include "lodepng.h"

#include <iostream>

Canvas::Canvas(uint32_t width, uint32_t height)
{
  this->width = width;
  this->height = height;

  Color c;
  c.Red = 255;
  c.Green = 255;
  c.Blue = 255;
  this->canvas.resize(width * height,c);
}

Canvas Canvas::open(const std::string& fileName)
{
  std::vector<unsigned char> rawImage;
  uint32_t rawImageWidth, rawImageHeight;
  lodepng::decode(rawImage, rawImageWidth, rawImageHeight, fileName);
  Canvas image(rawImageWidth, rawImageHeight);

  uint32_t count = 0;
  for(auto& c : image.canvas)
  {
    c.Red = rawImage[count + 0];
    c.Green = rawImage[count + 1];
    c.Blue = rawImage[count + 2];
    c.Alpha = rawImage[count + 3];

    count += 4;
  }

  return image;
}

void Canvas::save(const std::string& file_name)
{
  std::vector<unsigned char> image;
  image.resize(this->width * this->height * 4);

  uint32_t count = 0;
  for(auto& c : this->canvas)
  {
    image[count + 0] = c.Red;
    image[count + 1] = c.Green;
    image[count + 2] = c.Blue;
    image[count + 3] = c.Alpha;

    count += 4;
  }

  unsigned error = lodepng::encode(file_name.c_str(), image, this->width, this->height);
  if(error)
  {
    std::cout << "lodepng: encoder error " << error << ": " 
         << lodepng_error_text(error) << "\n";
  }
}

std::vector<Color>& Canvas::getCanvas() 
{
  return canvas;
}

uint32_t Canvas::getWidth()
{
  return width;
}

uint32_t Canvas::getHeight()
{
  return height;
}
