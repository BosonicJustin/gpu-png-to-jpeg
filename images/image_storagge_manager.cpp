//
// Created by Justin Zaliaduonis on 10/3/24.
//

#include "image_storagge_manager.h"
#include <opencv2/opencv.hpp>

/// TODO move to some global constants
const int RGB_COUNT = 3;

RawImage ImageStorageManager::loadImage(const std::string& imagePath) {
  cv::Mat image = cv::imread(imagePath);

  /// TODO ADD OTHER PROTECTIONS
  if(image.empty()) {
    throw std::invalid_argument("Image not found");
  }

  if (image.channels() != 3) {
    throw std::invalid_argument("Image must have 3 channels");
  }

  cvtColor(image, image, cv::COLOR_BGR2RGB);

  const size_t width = image.cols;
  const size_t height = image.rows;

  /// Calculating how much space will our image take up
  const size_t imageSize = width * height * image.channels();

  /// Here we are taking a pointer to a block of memory and assigning to it exact size of our image
  auto** imageData = new double*[height];
  for (size_t i = 0; i < height; ++i) {
    imageData[i] = new double[width * RGB_COUNT];
  }

  /// Convert to floating point since we will be moving from RGB to YCbCr which will have decimal places
  image.convertTo(image, CV_64F);

  /// Copying the data from the RGB image to our block of memory
  // std::memcpy(imageData, image.data, imageSize * sizeof(double));

  for (size_t i = 0; i < height; ++i) {
    std::memcpy(imageData[i], image.ptr<double>(i), width * RGB_COUNT * sizeof(double));
  }

  /// Free up the memory of an image
  image.release();

  return {imageData, RGB, height, width};
}
