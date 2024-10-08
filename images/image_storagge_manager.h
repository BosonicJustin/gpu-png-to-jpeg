#ifndef IMAGE_STORAGGE_MANAGER_H
#define IMAGE_STORAGGE_MANAGER_H

#include <string>

class ImageStorageManager {

  public:
    void loadImage(const std::string& imagePath, double** imageData, size_t* width, size_t* height);


};

#endif
