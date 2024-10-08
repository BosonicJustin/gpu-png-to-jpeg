#include <iostream>

#include "images/image_storagge_manager.h"
#include "images/raw_image.h"

int main() {
    RawImage raw_image = ImageStorageManager::loadImage("./static/leaf.png");

    // std::cout << imageHeight << " " << imageWidth << std::endl;
    //
    // for (size_t i = 0; i < imageHeight; i++) {
    //     for (size_t j = 0; j < imageWidth; j++) {
    //         for (size_t k = 0; k < 3; k++) {
    //             std::cou
    //         }
    //     }
    // }

    return 0;
}
