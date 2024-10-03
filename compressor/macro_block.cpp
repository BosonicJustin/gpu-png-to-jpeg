//
// Created by Justin Zaliaduonis on 10/3/24.
//

#include "macro_block.h"


bool is_index_out_of_bound(size_t x, size_t y, Color c) {
  return x < 0 || x >= _width || y < 0 || y >= _height || c < 0 || c > 2;
}

size_t get_index(size_t x, size_t y, Color c) {
  if (is_index_out_of_bound(x, y, c)) {
    throw std::invalid_argument("Coordinates out of bounds");
  }

  return (y * _width + x) * 3 + c;
}

MacroBlock::MacroBlock(size_t width, size_t height) {
  _width = width;
  _height = height;

  *_blockData = new float[_width * _height * 3];
}

/// TODO: Reuse color
/// TODO: reuse index calculator
void MacroBlock::setValue(size_t x, size_t y, size_t c, const float value) {
  *_blockData[get_index(x, y, c) * 3 + c] = value;

  _numberOfInsertions++;
}

bool MacroBlock::check_if_is_complete() {
  if (_numberOfInsertions != 3 * 8 * 8) {
    throw std::runtime_error("Unfilled block");
  }
}

MacroBlock::~MacroBlock() {
  delete[] _blockData;
}