#ifndef MACRO_BLOCK_H
#define MACRO_BLOCK_H

#include <cstddef>

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

private:
    double* _blockData;
    size_t _width;
    size_t _height;
    size_t _index;
    int _numberOfInsertions;

    size_t flattenIndex(size_t x, size_t y, size_t channel) const;
};

#endif //MACRO_BLOCK_H