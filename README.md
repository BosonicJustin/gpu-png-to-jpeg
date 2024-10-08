# JPEG-like Image Compression

This project implements a JPEG-like image compression algorithm in C++. It demonstrates the basic principles of JPEG compression, including color space conversion, discrete cosine transform (DCT), quantization, and run-length encoding.

## Features

- RGB to YCbCr color space conversion (ITU-R BT.601 standard)
- Image splitting into 8x8 MacroBlocks
- Discrete Cosine Transform (DCT) on MacroBlocks
- Quantization of DCT coefficients
- Zig-zag scanning of quantized coefficients
- Run-length encoding of zig-zag scanned data

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/jpeg-like-compression.git
   ```
2. Navigate to the project directory:
   ```
   cd jpeg-like-compression
   ```
3. Compile the project (assuming you have a C++ compiler and CMake installed):
   ```
   mkdir build && cd build
   cmake ..
   make
   ```

## Compression Process

1. **Color Space Conversion**: RGB to YCbCr using ITU-R BT.601 standard.
2. **Block Splitting**: Image is split into 8x8 pixel blocks.
3. **DCT**: Each block undergoes Discrete Cosine Transform.
4. **Quantization**: DCT coefficients are quantized to reduce data.
5. **Zig-zag Scanning**: Quantized coefficients are reordered.
6. **Run-length Encoding**: Final step to compress the data.

## Future Work

- Add Decoder
- Rewrite the process in CUDA