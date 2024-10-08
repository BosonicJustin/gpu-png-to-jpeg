#ifndef MACRO_BLOCK_H
#define MACRO_BLOCK_H

#include <cstddef>
#include <vector>


class MacroBlock {
public:
    static constexpr size_t CHANNELS = 3;

    MacroBlock(size_t width, size_t height, size_t index);
    ~MacroBlock();

    // Prevent copying
    MacroBlock(const MacroBlock&) = delete;
    MacroBlock& operator=(const MacroBlock&) = delete;

    // Allow moving
    MacroBlock(MacroBlock&& other) noexcept;
    MacroBlock& operator=(MacroBlock&& other) noexcept;

    void setData(size_t x, size_t y, size_t channel, double value);
    void setData(size_t flattenedIndex, double value);
    double getData(size_t x, size_t y, size_t channel) const;
    double getData(size_t flattenedIndex) const;
    bool check_if_is_complete() const;
    size_t getIndex() const;
    size_t getWidth() const;
    size_t getHeight() const;

    MacroBlock to_frequency_domain() const;

    void quantize() const;

private:
    double* _blockData;
    size_t _width;
    size_t _height;
    size_t _index;
    int _numberOfInsertions;

    size_t _flatten_index(size_t x, size_t y, size_t channel) const;

    double _get_fequency_value(size_t u, size_t v, size_t channel) const;

    /// Quantization matrix
    std::vector<int> Q = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
    };
};

#endif //MACRO_BLOCK_H