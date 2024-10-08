#include <iostream>
#include <opencv2/opencv.hpp>
#include "images/image_storage_manager.h"
#include "images/raw_image.h"

int main() {
    RawImage raw_image = ImageStorageManager::loadImage("./static/leaf.png");

    raw_image.to_ycbcr();

    return 0;
}