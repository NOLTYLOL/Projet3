
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

//modify
void hide_bit(BmpImage& bmp, int x, int y, unsigned char bit_to_hide) //test
{
    if (x < 0 || x >= bmp.width ||
        y < 0 || y >= bmp.height)
    {
        throw std::out_of_range("Invalid pixel coordinates");
    }

    if (bit_to_hide > 1)
    {
        throw std::invalid_argument(
            "The bit to hide must be 0 or 1");
    }

    constexpr std::size_t bytes_per_pixel = 3;

    const std::size_t row_offset =
        static_cast<std::size_t>(bmp.pixel_offset) +
        static_cast<std::size_t>(y) * bmp.stride;

    const std::size_t blue_offset =
        row_offset + static_cast<std::size_t>(x) * bytes_per_pixel;

    unsigned char blue =
        static_cast<unsigned char>(bmp.image_bytes[blue_offset]);

    blue = static_cast<unsigned char>(
        (blue & 0xFE) | bit_to_hide);

    bmp.image_bytes[blue_offset] = static_cast<char>(blue);
}

//modify
void hide_octet(BmpImage& bmp, int x, int y, unsigned char octet)
{
    if (y < 0 || y >= bmp.height || x < 0 || x + 7 >= bmp.width)
    {
        throw std::out_of_range("Cannot hide a byte at this position");
    }

    constexpr std::size_t bytes_per_pixel = 3;
    const std::size_t row_offset =
        static_cast<std::size_t>(bmp.pixel_offset) +
        static_cast<std::size_t>(y) * bmp.stride;

    for (int bit_index = 0; bit_index < 8; ++bit_index)
    {
        const std::size_t blue_offset =
            row_offset +
            static_cast<std::size_t>(x + bit_index) * bytes_per_pixel;

        unsigned char blue = static_cast<unsigned char>(
            bmp.image_bytes[blue_offset]);
        const unsigned char bit = static_cast<unsigned char>(
            (octet >> (7 - bit_index)) & 0x01);

        blue = static_cast<unsigned char>((blue & 0xFE) | bit);
        bmp.image_bytes[blue_offset] = static_cast<char>(blue);
    }
}

//modify
unsigned char extraire_octet(const BmpImage& bmp, int x, int y)
{
    if (y < 0 || y >= bmp.height || x < 0 || x + 7 >= bmp.width)
    {
        throw std::out_of_range("Cannot extract a byte at this position");
    }

    constexpr std::size_t bytes_per_pixel = 3;
    const std::size_t row_offset =
        static_cast<std::size_t>(bmp.pixel_offset) +
        static_cast<std::size_t>(y) * bmp.stride;

    unsigned char octet = 0;

    for (int bit_index = 0; bit_index < 8; ++bit_index)
    {
        const std::size_t blue_offset =
            row_offset +
            static_cast<std::size_t>(x + bit_index) * bytes_per_pixel;

        const unsigned char blue = static_cast<unsigned char>(
            bmp.image_bytes[blue_offset]);
        const unsigned char bit = blue & 0x01;

        octet = static_cast<unsigned char>((octet << 1) | bit);
    }

    return octet;
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