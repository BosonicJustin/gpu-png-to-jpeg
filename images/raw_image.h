#ifndef RAW_IMAGE_H
#define RAW_IMAGE_H

#include <cstddef>
#include "../general/colors.h"
#include "../general/encoding.h"

class RawImage {
public:
    RawImage(Encoding enc, size_t height, size_t width);

    double get_pixel(size_t x, size_t y, ColorRGB c) const;
    double get_pixel(size_t x, size_t y, ColorYCbCr c) const;
    void set_pixel(size_t x, size_t y, ColorRGB c, double value);
    void set_pixel(size_t x, size_t y, ColorYCbCr c, double value);

    size_t get_height() const;
    size_t get_width() const;

    ~RawImage();

    // Prevent copying
    RawImage(const RawImage&) = delete;
    RawImage& operator=(const RawImage&) = delete;

    // Allow moving
    RawImage(RawImage&& other) noexcept;
    RawImage& operator=(RawImage&& other) noexcept;

    void to_ycbcr();

private:
    size_t _get_index(size_t x, size_t y, ColorRGB c) const;
    size_t _get_index(size_t x, size_t y, ColorYCbCr c) const;
    size_t _get_index_unsafe(size_t x, size_t y, size_t c) const;
    bool _is_index_out_of_bounds(size_t x, size_t y, size_t c) const;

    double* _imageData;
    Encoding _encoding;
    size_t _height, _width;
};

#endif // RAW_IMAGE_H