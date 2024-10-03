#ifndef IMAGE_STORAGGE_MANAGER_H
#define IMAGE_STORAGGE_MANAGER_H


class ImageStorageManager {

  public:
    void loadImage(const std::string& imagePath, unsigned char** imageData, int* width, int* height);


};

#endif
