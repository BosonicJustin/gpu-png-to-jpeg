#include "raw_image.h"
#include "../compressor/macro_block.h"

/// ITU-R BT.601 constant set
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

void RawImage::setPixel(size_t x, size_t y, Color c, double value) {
  (*_imgData)[get_index(x, y, c)] = value;
}

/// https://en.wikipedia.org/wiki/YCbCr#RGB_conversion
void RawImage::to_ycbcr() {
  if (_encoding == YCbCr) {
    return;
  }

  for (size_t y = 0; y < _height; ++y) {
    for (size_t x = 0; x < _width; ++x) {
      double R = getPixel(x, y, RED);
      double G = getPixel(x, y, GREEN);
      double B = getPixel(x, y, BLUE);

      double Y = R * K_R + G * K_G + B * K_B;
      double Pb = 0.5 * ((B - Y) / (1 - K_B));
      double Pr = 0.5 * ((R - Y) / (1 - K_R));

      setPixel(x, y, Y_c, Y);
      setPixel(x, y, Pb_c, Pb);
      setPixel(x, y, Pr_c, Pr);
    }
  }

  _encoding = YCbCr;
}

/// https://en.wikipedia.org/wiki/YCbCr#RGB_conversion
void RawImage::to_rgb() {
  if (_encoding == RGB) {
    return;
  }

  for (size_t y = 0; y < _height; ++y) {
    for (size_t x = 0; x < _width; ++x) {
      double Y = getPixel(x, y, Y_c);
      double Pb = getPixel(x, y, Pb_c);
      double Pr = getPixel(x, y, Pr_c);

      /// TODO: Double-check math
      double R = Y + Pr * (2 - 2 * K_R);
      double G = Y - Pb * (K_B / K_G) * (2 - 2 * K_B) - P_R * (K_R / K_G) * (2 - 2 * K_R);
      double B = Y - Pb * (2 - 2 * K_B);

      setPixel(x, y, RED, R);
      setPixel(x, y, GREEN, G);
      setPixel(x, y, BLUE, B);
    }
  }

  _encoding = RGB;
}

double get_val_with_fallback(size_t x, size_t y, Color c) {
    if (is_index_out_of_bound(x, y, c)) {
      return 0;
    }

    return _imgData(y * _width + x) * 3 + c;
}

**MacroBlocks split_to_blocks() {
  size_t number_of_horizontal_blocks = std::ceil(_width / 8);
  size_t number_of_vertical_blocks = std::ceil(_height / 8);

  *MacroBlocks macroBlocks = new MacroBlock[number_of_horizontal_blocks * number_of_vertical_blocks];

  for (size_t y = 0; y < number_of_vertical_blocks; ++y) {
     for (size_t x = 0; x < number_of_horizontal_blocks; ++x) {
       *float blockData = new float[8 * 8 * 3];

       MacroBlock block = new MacroBlock(8, 8);

       for (size_t y_interval_index = 8 * y, y_interval_index < 8 * (y + 1); y_interval_index++) {
         for (size_t x_interval_index = 8 * x, x_interval_index < 8 * (x + 1); x_interval_index++) {
           for (size_t c = 0; c < 3: c++) {
             size_t unshifted_y = y_interval_index - 8 * y;
             size_t unshifted_x = x_interval_index - 8 * x;

             block.set_pixel_value(unshifted_x, unshifted_y, c, get_val_with_fallback(unshifted_x, unshifted_y, c));
           }
         }
       }

       block.check_if_is_complete();
       macroBlocks[(y * number_of_horizontal_blocks + x)] = block;
     }
  }

  return macroBlocks;
}