cmake_minimum_required(VERSION 3.10)
project(gpu-png-to-jpeg)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your executable
add_executable(
        png-to-jpeg
        main.cpp
		images/image_storage_manager.cpp
		images/image_storage_manager.h
        images/raw_image.cpp
        images/raw_image.h
     #   compressor/macro_block.cpp
      #  compressor/macro_block.h
)

# Link OpenCV libraries
target_link_libraries(png-to-jpeg ${OpenCV_LIBS})

