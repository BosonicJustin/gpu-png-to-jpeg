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

    std::vector<std::pair<std::vector<double>, std::vector<size_t>>> encode();

    /// [color] -> ([frequencies], [repeated])

private:
    double* _blockData;
    size_t _width;
    size_t _height;
    size_t _index;
    int _numberOfInsertions;

    size_t _flatten_index(size_t x, size_t y, size_t channel) const;

    double _get_frequency_value(size_t u, size_t v, size_t channel) const;

    std::vector<double> _get_zig_zag_frequency(size_t c);

    static std::pair<std::vector<double>, std::vector<size_t>> MacroBlock::_run_length_encode_frequency(
        const std::vector<double>& zig_zag_frequency
    );

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

    const std::array<size_t, 64> ZIG_ZAG_ORDER = {
        0,  1,  5,  6, 14, 15, 27, 28,
        2,  4,  7, 13, 16, 26, 29, 42,
        3,  8, 12, 17, 25, 30, 41, 43,
        9, 11, 18, 24, 31, 40, 44, 53,
       10, 19, 23, 32, 39, 45, 52, 54,
       20, 22, 33, 38, 46, 51, 55, 60,
       21, 34, 37, 47, 50, 56, 59, 61,
       35, 36, 48, 49, 57, 58, 62, 63
    };
};

#endif //MACRO_BLOCK_H