//
// Created by Justin Zaliaduonis on 10/3/24.
//

#include "image_storagge_manager.h"
#include <opencv2/opencv.hpp>

const int RGB_COUNT = 3;

void ImageStorageManager::loadImage(const std::string& imagePath, double** imageData, int* width, int* height) {
  cv::Mat image = cv::imread(imagePath);

  if(image.empty()) {
    throw std::invalid_argument("Image not found");
  }

  cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

  *width = image.cols;
  *height = image.rows;

  /// Calculating how much space will our image take up
  unsigned int imageSize = (*width) * (*height) * RGB_COUNT;

  /// Here we are taking a pointer to a block of memory and assigning to it exact size of our image
  *imageData = new double [imageSize];

  /// Copying the data from the RGB image to our block of memory
  std::memcpy(*imageData, image.data, imageSize * sizeof(double));

  /// Free up the memory of an image
  image.release();
}