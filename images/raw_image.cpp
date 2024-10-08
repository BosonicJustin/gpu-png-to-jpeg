#include "raw_image.h"

#include <algorithm>
#include <stdexcept>
#include <cstring> // for std::memset

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