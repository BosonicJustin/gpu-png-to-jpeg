#include "macro_block.h"
#include <stdexcept>

MacroBlock::MacroBlock(size_t width, size_t height, size_t index)
    : _width(width), _height(height), _index(index), _numberOfInsertions(0) {
    _blockData = new double[width * height * CHANNELS];
}

MacroBlock::~MacroBlock() {
    delete[] _blockData;
}

MacroBlock::MacroBlock(MacroBlock&& other) noexcept
    : _blockData(other._blockData), _width(other._width), _height(other._height),
      _index(other._index), _numberOfInsertions(other._numberOfInsertions) {
    other._blockData = nullptr;
}

MacroBlock& MacroBlock::operator=(MacroBlock&& other) noexcept {
    if (this != &other) {
        delete[] _blockData;
        _blockData = other._blockData;
        _width = other._width;
        _height = other._height;
        _index = other._index;
        _numberOfInsertions = other._numberOfInsertions;
        other._blockData = nullptr;
    }
    return *this;
}

size_t MacroBlock::flattenIndex(size_t x, size_t y, size_t channel) const {
    return (y * _width + x) * CHANNELS + channel;
}

void MacroBlock::setData(size_t x, size_t y, size_t channel, double value) {
    if (x >= _width || y >= _height || channel >= CHANNELS) {
        throw std::out_of_range("Coordinates out of range");
    }
    size_t index = flattenIndex(x, y, channel);
    _blockData[index] = value;
    ++_numberOfInsertions;
}

void MacroBlock::setData(size_t flattenedIndex, double value) {
    if (flattenedIndex >= _width * _height * CHANNELS) {
        throw std::out_of_range("Flattened index out of range");
    }
    _blockData[flattenedIndex] = value;
    ++_numberOfInsertions;
}

double MacroBlock::getData(size_t x, size_t y, size_t channel) const {
    if (x >= _width || y >= _height || channel >= CHANNELS) {
        throw std::out_of_range("Coordinates out of range");
    }
    size_t index = flattenIndex(x, y, channel);
    return _blockData[index];
}

double MacroBlock::getData(size_t flattenedIndex) const {
    if (flattenedIndex >= _width * _height * CHANNELS) {
        throw std::out_of_range("Flattened index out of range");
    }
    return _blockData[flattenedIndex];
}

bool MacroBlock::check_if_is_complete() const {
    return _numberOfInsertions == _width * _height * CHANNELS;
}

size_t MacroBlock::getIndex() const {
    return _index;
}

size_t MacroBlock::getWidth() const {
    return _width;
}

size_t MacroBlock::getHeight() const {
    return _height;
}