#include <iostream>

#include "images/image_storagge_manager.h"
#include "images/raw_image.h"

int main() {
    size_t imageHeight = 0;
    size_t imageWidth = 0;
    double *imageData = nullptr;
    ImageStorageManager imageStorageManager;

    imageStorageManager.loadImage("./static/leaf.png", &imageData, &imageHeight, &imageWidth);

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
