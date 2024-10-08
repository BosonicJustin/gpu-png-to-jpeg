#include <iostream>
#include "images/image_storage_manager.h"
#include "images/raw_image.h"

int main() {
    RawImage raw_image = ImageStorageManager::loadImage("./static/leaf.png");

    raw_image.to_ycbcr();

    std::vector<MacroBlock> blocks = raw_image.split_to_macro_blocks();
    std::vector<std::vector<std::pair<std::vector<double>, std::vector<size_t>>>> latent_encodings;

    for (auto &block : blocks) {
        block = block.to_frequency_domain();
        block.quantize();
        latent_encodings.push_back(block.encode());
    }

    // Measure size of latent_encodings in bits
    size_t bits_per_byte = 8;
    size_t latent_size_bits = 0;
    for (const auto& block_encoding : latent_encodings) {
        for (const auto& channel_encoding : block_encoding) {
            latent_size_bits += channel_encoding.first.size() * sizeof(double) * bits_per_byte;  // frequencies
            latent_size_bits += channel_encoding.second.size() * sizeof(size_t) * bits_per_byte; // repetitions
        }
    }

    // Measure size of raw_image._imageData in bits
    size_t raw_size_bits = raw_image.get_width() * raw_image.get_height() * 3 * sizeof(double) * bits_per_byte;

    std::cout << "Size of latent encodings: " << latent_size_bits << " bits" << std::endl;
    std::cout << "Size of raw image data: " << raw_size_bits << " bits" << std::endl;

    // Calculate compression ratio
    double compression_ratio = static_cast<double>(raw_size_bits) / latent_size_bits;
    std::cout << "Compression ratio: " << compression_ratio << ":1" << std::endl;

    return 0;
}