#ifndef IMAGE_STORAGGE_MANAGER_H
#define IMAGE_STORAGGE_MANAGER_H

#include <string>

#include "raw_image.h"

class ImageStorageManager {

  public:
    static RawImage loadImage(const std::string& imagePath);


};

#endif
