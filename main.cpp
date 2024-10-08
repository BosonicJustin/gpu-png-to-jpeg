#include <iostream>
#include "images/image_storage_manager.h"
#include "images/raw_image.h"

int main() {
    RawImage raw_image = ImageStorageManager::loadImage("./static/leaf.png");

    raw_image.to_ycbcr();

    std::vector<MacroBlock> blocks = raw_image.split_to_macro_blocks();

    for (auto & block : blocks) {
        block = block.to_frequency_domain();
    }

    return 0;
}