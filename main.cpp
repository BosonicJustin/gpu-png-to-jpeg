#include <iostream>

#include "image_storage_manager.h";

int main() {
    size_t imageHeight = 0;
    size_t imageWidth = 0;
    double *imageData = NULL;
    ImageStorageManager imageStorageManager;

    imageStorageManager.loadImageFromFile("./static/leaf.jpg", &imageData, &imageHeight, &imageWidth);

    return 0;
}
