#pragma once

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

void save_bmp(const std::string& output_path, const BmpImage& bmp);

void copy_bmp(const std::string& source_path, const std::string& output_path);