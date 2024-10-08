#include "macro_block.h"
#include <stdexcept>

#include <cmath>


double double_cos(const size_t x, const size_t y, const size_t u, const size_t v) {
    return cos((2 * x + 1) * u * M_PI / 16) * cos((2 * y + 1) * v * M_PI / 16);
}

double alpha(const size_t u) {
    if (u == 0) {
        return 1 / sqrt(2);
    }

    return 1;
}

double MacroBlock::_get_frequency_value(size_t u, size_t v, size_t channel) const {
    double f = 0;
    double alpha_1 = alpha(u);
    double alpha_2 = alpha(v);

    for (size_t y = 0; y < u; ++y) {
        for (size_t x = 0; x < v; ++x) {
            /// TODO: center data at 0
            f += 0.25 * alpha_1 * alpha_2 * getData(x, y, channel) * double_cos(x, y, u, v);
        }
    }

    return f;
}

void MacroBlock::quantize() const {
    for (size_t x = 0; x < _width; ++x) {
        for (size_t y = 0; y < _height; ++y) {
            int q = Q[y * 8 + x];

            for (size_t c = 0 ; c < 3; ++c) {
                _blockData[_flatten_index(x, y, c)] = std::round(getData(x, y, c) / static_cast<double>(q));;
            }
        }
    }
}

MacroBlock MacroBlock::to_frequency_domain() const {
    MacroBlock block(_width, _height, _index);

    /// This is very slow, maybe there is some variant iFFT that fulfills this
    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            for (size_t channel = 0; channel < 3; ++channel) {
                block.setData(x, y, channel, _get_frequency_value(x, y, channel));
            }
        }
    }

    if (!block.check_if_is_complete()) {
        throw std::runtime_error("Macro block not fully completed");
    }

    return block;
}

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

size_t MacroBlock::_flatten_index(size_t x, size_t y, size_t channel) const {
    return (y * _width + x) * CHANNELS + channel;
}

void MacroBlock::setData(size_t x, size_t y, size_t channel, double value) {
    if (x >= _width || y >= _height || channel >= CHANNELS) {
        throw std::out_of_range("Coordinates out of range");
    }
    size_t index = _flatten_index(x, y, channel);
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
    size_t index = _flatten_index(x, y, channel);
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

std::vector<double> MacroBlock::_get_zig_zag_frequency(const size_t c) {
    std::vector<double> zig_zag_frequency(64);

    for (size_t i = 0; i < 64; ++i) {
        zig_zag_frequency[i] = _blockData[ZIG_ZAG_ORDER[i] * CHANNELS + c];
    }

    return zig_zag_frequency;
}

std::pair<std::vector<double>, std::vector<size_t>> MacroBlock::_run_length_encode_frequency(
    const std::vector<double>& zig_zag_frequency
) {
    std::vector<double> frequencies;
    std::vector<size_t> repetitions;

    frequencies.push_back(zig_zag_frequency[0]);
    repetitions.push_back(1);

    for (size_t i = 1; i < zig_zag_frequency.size(); ++i) {
        if (zig_zag_frequency[i] == frequencies.back()) {
            repetitions.back() += 1;
        } else {
            frequencies.push_back(zig_zag_frequency[i]);
            repetitions.push_back(1);
        }
    }

    if (frequencies.size() != repetitions.size()) {
        throw std::runtime_error("Size mismatch");
    }

    return std::make_pair(frequencies, repetitions);
}

std::vector<std::pair<std::vector<double>, std::vector<size_t>>> MacroBlock::encode() {
    std::vector<std::pair<std::vector<double>, std::vector<size_t>>> zz_freq_pairs_per_color(3);

    for (size_t c = 0; c < 3; ++c) {
        auto zig_zag_frequency = _get_zig_zag_frequency(c);
        zz_freq_pairs_per_color[c] = _run_length_encode_frequency(zig_zag_frequency);
    }

    return zz_freq_pairs_per_color;
}