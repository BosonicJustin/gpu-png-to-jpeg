//
// Created by Justin Zaliaduonis on 10/3/24.
//

#ifndef RAW_IMAGE_H
#define RAW_IMAGE_H

#include <stddef.h>

/// TODO MOVE TO CONSTATNS
enum Encoding {
    RGB = 1,
    YCbCr = 2,
};

enum ColorRGB {
  RED,
  GREEN,
  BLUE,
};

enum ColorYCbCr {
  Y_c,
  Pb_c,
  Pr_c,
};

class RawImage {
    public:
        RawImage(
          double** imgData, const Encoding enc, const size_t height, const size_t width
          ): _imageData(imgData), _encoding(enc), _height(height), _width(width) {};

//        void to_ycbcr();
//        void to_rgb();

//        void setPixel(size_t x, size_t y, Color c, double value);
//        double get_pixel(size_t x, size_t y, Color c) const;

        /// Now the splitting is implemented to be default 8x8 - no subsampling
//        **MacroBlock split_to_blocks();

        ~RawImage();

    private:
        double** _imageData;
        Encoding _encoding;
        size_t _height, _width;
};


#endif
