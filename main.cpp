#include <iostream>

#include "images/image_storagge_manager.h"

int main() {
    size_t imageHeight = 0;
    size_t imageWidth = 0;
    double *imageData = NULL;
    ImageStorageManager imageStorageManager;

    imageStorageManager.loadImage("./static/leaf.png", &imageData, &imageHeight, &imageWidth);

    std::cout << "Hello" << std::endl;

    return 0;
}
