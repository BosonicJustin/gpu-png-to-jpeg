#include "raw_image.h"

float const K_R = 0.299;
float const K_G = 0.587;
float const K_B = 0.114;


bool is_index_out_of_bound(size_t x, size_t y, Color c) {
  return x < 0 || x >= _width || y < 0 || y >= _height || c < 0 || c > 2;
}

size_t get_index(size_t x, size_t y, Color c) {
  if (is_index_out_of_bound(x, y, c)) {
    throw std::invalid_argument("Coordinates out of bounds");
  }

  return (y * _width + x) * 3 + c;
}

RawImage::~RawImage() {
  delete[] _imgData;
}

double RawImage::getPixel(size_t x, size_t y, Color c) {
  return (*_imgData)[get_index(x, y, c)];
}

void RawImage::to_ycbcr() {
  if (_encoding == YCbCr) {
    return;
  }


}