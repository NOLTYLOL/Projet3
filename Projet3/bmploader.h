#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct BmpImage
{
    std::vector<char> image_bytes;
    int pixel_offset = 0;
    int width = 0;
    int height = 0;
    std::size_t stride = 0;
};

BmpImage load_bmp(const std::string& file_path);

// [modify -->
void hide_bit(BmpImage& bmp, int x, int y, unsigned char bit_to_hide);
void hide_octet(BmpImage& bmp, int x, int y, unsigned char octet);
unsigned char extraire_octet(const BmpImage& bmp, int x, int y); 
// <-- modify]

void save_bmp(const std::string& output_path, const BmpImage& bmp);