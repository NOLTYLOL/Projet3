#include "bmploader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

BmpImage load_bmp(const std::string& file_path)
{
    std::ifstream read_file(file_path, std::ios::binary);

    if (!read_file.is_open())
    {
        throw std::runtime_error("File can't be opened: " + file_path);
    }

    char signature[2];
    read_file.read(signature, 2);

    if (read_file.gcount() != 2 || signature[0] != 'B' || signature[1] != 'M')
    {
        throw std::runtime_error("Not a valid BMP file");
    }

    char buffer[4];

    read_file.seekg(10, std::ios::beg);
    read_file.read(buffer, 4);
    int pixel_offset =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8) |
        (static_cast<std::uint8_t>(buffer[2]) << 16) |
        (static_cast<std::uint8_t>(buffer[3]) << 24);

    read_file.seekg(14, std::ios::beg);
    read_file.read(buffer, 4);
    int dib_size =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8) |
        (static_cast<std::uint8_t>(buffer[2]) << 16) |
        (static_cast<std::uint8_t>(buffer[3]) << 24);

    read_file.seekg(18, std::ios::beg);
    read_file.read(buffer, 4);
    int width =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8) |
        (static_cast<std::uint8_t>(buffer[2]) << 16) |
        (static_cast<std::uint8_t>(buffer[3]) << 24);

    read_file.seekg(22, std::ios::beg);
    read_file.read(buffer, 4);
    int height =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8) |
        (static_cast<std::uint8_t>(buffer[2]) << 16) |
        (static_cast<std::uint8_t>(buffer[3]) << 24);

    read_file.seekg(26, std::ios::beg);
    read_file.read(buffer, 2);
    int planes =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8);

    read_file.seekg(28, std::ios::beg);
    read_file.read(buffer, 2);
    int bits_per_pixel =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8);

    read_file.seekg(30, std::ios::beg);
    read_file.read(buffer, 4);
    int compression =
        static_cast<std::uint8_t>(buffer[0]) |
        (static_cast<std::uint8_t>(buffer[1]) << 8) |
        (static_cast<std::uint8_t>(buffer[2]) << 16) |
        (static_cast<std::uint8_t>(buffer[3]) << 24);

    if (dib_size != 40 || width <= 0 || height <= 0 ||
        planes != 1 || bits_per_pixel != 24 || compression != 0)
    {
        throw std::runtime_error("Unsupported BMP format (must be 24-bit uncompressed)");
    }

    read_file.clear();
    read_file.seekg(0, std::ios::end);
    std::streampos file_size = read_file.tellg();

    read_file.seekg(0, std::ios::beg);
    std::vector<char> image_bytes(static_cast<std::size_t>(file_size));
    read_file.read(image_bytes.data(), image_bytes.size());
    read_file.close();

    std::size_t stride = (width * 3 + 3) / 4 * 4;

    if (pixel_offset < 54 || pixel_offset + height * stride > image_bytes.size())
    {
        throw std::runtime_error("bmp file is cuttted or corrupted");
    }

    BmpImage bmp;
    bmp.image_bytes = image_bytes;
    bmp.pixel_offset = pixel_offset;
    bmp.width = width;
    bmp.height = height;
    bmp.stride = stride;

    return bmp;
}

void save_bmp(const std::string& output_path, const BmpImage& bmp)
{
    std::ifstream existing_file(output_path, std::ios::binary);
    if (existing_file.is_open())
    {
        existing_file.close();
        throw std::runtime_error("Destination file already exists: " + output_path);
    }

    std::ofstream write_file(output_path, std::ios::binary);

    if (!write_file.is_open())
    {
        throw std::runtime_error("Output file can't be opened: " + output_path);
    }

    write_file.write(bmp.image_bytes.data(), bmp.image_bytes.size());
    write_file.close();
}

void copy_bmp(const std::string& source_path, const std::string& output_path)
{
    BmpImage bmp = load_bmp(source_path);
    save_bmp(output_path, bmp);
}