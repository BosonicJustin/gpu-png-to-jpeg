#include "image_storage_manager.h"
#include <opencv2/opencv.hpp>

/// TODO move to some global constants


RawImage ImageStorageManager::loadImage(const std::string& imagePath) {
  cv::Mat image = cv::imread(imagePath);

  if(image.empty()) {
    throw std::invalid_argument("Image not found");
  }

  if (image.channels() != 3) {
    throw std::invalid_argument("Image must have 3 channels");
  }

  cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

  const size_t width = image.cols;
  const size_t height = image.rows;

  // Create a RawImage object
  RawImage rawImage(RGB, height, width);

  // Convert to double precision without scaling
  image.convertTo(image, CV_64F);

  // Copy data from cv::Mat to RawImage
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      cv::Vec3d pixel = image.at<cv::Vec3d>(y, x);
      rawImage.set_pixel(x, y, RED, pixel[0]);
      rawImage.set_pixel(x, y, GREEN, pixel[1]);
      rawImage.set_pixel(x, y, BLUE, pixel[2]);
    }
  }

  // Free up the memory of the OpenCV image
  image.release();

  return rawImage;
}