#include "raw_image.h"

#include <stdexcept>
#include <cstring> // for std::memset

// Constants for YCbCr conversion (BT.601 standard)
// https://en.wikipedia.org/wiki/YCbCr#ITU-R_BT.601_conversion
// Saving the constant at compile time
constexpr double KR = 0.299;
constexpr double KG = 0.587;
constexpr double KB = 0.114;

RawImage::RawImage(const Encoding enc, const size_t height, const size_t width)
    : _encoding(enc), _height(height), _width(width) {
    _imageData = new double[height * width * 3];
    std::memset(_imageData, 0, height * width * 3 * sizeof(double));
}

#include <iostream>

bool RawImage::_is_index_out_of_bounds(size_t x, size_t y, size_t c) const {
    return x >= _width || y >= _height || c > 2;
}

size_t RawImage::_get_index_unsafe(size_t x, size_t y, size_t c) const {
    return (y * _width + x) * 3 + c;
}

size_t RawImage::_get_index(size_t x, size_t y, ColorRGB c) const {
    return _get_index_unsafe(x, y, static_cast<size_t>(c));
}

size_t RawImage::_get_index(size_t x, size_t y, ColorYCbCr c) const {
    return _get_index_unsafe(x, y, static_cast<size_t>(c));
}

RawImage::~RawImage() {
    delete[] _imageData;
}

size_t RawImage::get_height() const {
    return _height;
}

size_t RawImage::get_width() const {
    return _width;
}

double RawImage::get_pixel(size_t x, size_t y, ColorRGB c) const {
    if (_is_index_out_of_bounds(x, y, static_cast<size_t>(c))) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return _imageData[_get_index(x, y, c)];
}

double RawImage::get_pixel(size_t x, size_t y, ColorYCbCr c) const {
    if (_is_index_out_of_bounds(x, y, static_cast<size_t>(c))) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    return _imageData[_get_index(x, y, c)];
}

void RawImage::set_pixel(size_t x, size_t y, ColorRGB c, double value) {
    if (_is_index_out_of_bounds(x, y, static_cast<size_t>(c))) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    _imageData[_get_index(x, y, c)] = value;
}

void RawImage::set_pixel(size_t x, size_t y, ColorYCbCr c, double value) {
    if (_is_index_out_of_bounds(x, y, static_cast<size_t>(c))) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    _imageData[_get_index(x, y, c)] = value;
}

RawImage::RawImage(RawImage&& other) noexcept
    : _imageData(other._imageData), _encoding(other._encoding),
      _height(other._height), _width(other._width) {
    other._imageData = nullptr;
}

RawImage& RawImage::operator=(RawImage&& other) noexcept {
    if (this != &other) {
        delete[] _imageData;
        _imageData = other._imageData;
        _encoding = other._encoding;
        _height = other._height;
        _width = other._width;
        other._imageData = nullptr;
    }
    return *this;
}

void RawImage::to_ycbcr() {
    if (_encoding == YCbCr) {
        return;
    }

    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            double R = get_pixel(x, y, RED);
            double G = get_pixel(x, y, GREEN);
            double B = get_pixel(x, y, BLUE);

            // Convert to YCbCr
            double Y = KR * R + KG * G + KB * B;
            double Cb = 0.5 * ((B - Y) / (1.0 - KB));
            double Cr = 0.5 * ((R - Y) / (1.0 - KR));

            // Store YCbCr values
            set_pixel(x, y, Y_c, Y);
            set_pixel(x, y, Cb_c, Cb);
            set_pixel(x, y, Cr_c, Cr);
        }
    }

    _encoding = YCbCr;
}


std::vector<MacroBlock> RawImage::split_to_macro_blocks() const {
    constexpr size_t block_size = 8; // For now hardcoding as we are doing no upsampling
    constexpr size_t channels = 3;   // RGB or YCbCr

    auto num_blocks_width = static_cast<size_t>(std::ceil(static_cast<double>(_width) / block_size));
    auto num_blocks_height = static_cast<size_t>(std::ceil(static_cast<double>(_height) / block_size));

    std::vector<MacroBlock> macro_blocks;
    macro_blocks.reserve(num_blocks_width * num_blocks_height);

    for (size_t by = 0; by < num_blocks_height; ++by) {
        for (size_t bx = 0; bx < num_blocks_width; ++bx) {
            size_t block_index = by * num_blocks_width + bx; // Flattening the 2D index into 1

            MacroBlock block(block_size, block_size, block_index);

            for (size_t y = 0; y < block_size; ++y) {
                for (size_t x = 0; x < block_size; ++x) {
                    // Computing intervals to take from the original image
                    size_t img_x = bx * block_size + x;
                    size_t img_y = by * block_size + y;

                    if (img_x < _width && img_y < _height) {
                        // For each color channel
                        for (size_t c = 0; c < channels; ++c) {
                            double value;
                            if (_encoding == Encoding::RGB) {
                                value = get_pixel(img_x, img_y, static_cast<ColorRGB>(c));
                            } else { // YCbCr
                                value = get_pixel(img_x, img_y, static_cast<ColorYCbCr>(c));
                            }
                            block.setData(x, y, c, value);
                        }
                    } else {
                        /// If we go out of bounds - we set 0
                        /// This happens if image dimension is not a multiple of 8

                        for (size_t c = 0; c < channels; ++c) {
                            block.setData(x, y, c, 0.0);
                        }

                    }
                }
            }

            if (!block.check_if_is_complete()) {
                throw std::runtime_error("MacroBlock is not complete after filling");
            }

            macro_blocks.push_back(std::move(block));
        }
    }

    return macro_blocks;
}