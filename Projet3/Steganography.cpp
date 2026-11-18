#include "steganography.h"
#include <stdexcept>
#include <vector>


static std::vector<std::size_t> get_valid_pixel_indices(const BmpImage& bmp)
{
    std::vector<std::size_t> indices;
    std::size_t row_bytes = static_cast<std::size_t>(bmp.width) * 3;
    indices.reserve(row_bytes * bmp.height);

    for (int y = 0; y < bmp.height; ++y)
    {
        std::size_t row_start = bmp.pixel_offset + y * bmp.stride;
        for (std::size_t x = 0; x < row_bytes; ++x)
        {
            indices.push_back(row_start + x);
        }
    }
    return indices;
}

void hide_message(BmpImage& bmp, const std::string& message)
{
    std::vector<std::size_t> pixel_indices = get_valid_pixel_indices(bmp);

   
    std::uint32_t msg_length = static_cast<std::uint32_t>(message.size());
    std::size_t required_bits = 32 + (msg_length * 8);

    if (required_bits > pixel_indices.size())
    {
        throw std::runtime_error("L'image est trop petite pour contenir ce message.");
    }

    std::size_t bit_cursor = 0;

   
    for (int i = 0; i < 32; ++i)
    {
        std::uint8_t bit = (msg_length >> i) & 1;
        std::size_t byte_idx = pixel_indices[bit_cursor++];

        
        bmp.image_bytes[byte_idx] = (bmp.image_bytes[byte_idx] & 0xFE) | bit;
    }

    
    for (char c : message)
    {
        for (int i = 0; i < 8; ++i)
        {
            std::uint8_t bit = (static_cast<std::uint8_t>(c) >> i) & 1;
            std::size_t byte_idx = pixel_indices[bit_cursor++];

            bmp.image_bytes[byte_idx] = (bmp.image_bytes[byte_idx] & 0xFE) | bit;
        }
    }
}

std::string extract_message(const BmpImage& bmp)
{
    std::vector<std::size_t> pixel_indices = get_valid_pixel_indices(bmp);

    if (pixel_indices.size() < 32)
    {
        throw std::runtime_error("L'image est trop petite pour contenir un message valide.");
    }

    std::size_t bit_cursor = 0;
    std::uint32_t msg_length = 0;

    
    for (int i = 0; i < 32; ++i)
    {
        std::size_t byte_idx = pixel_indices[bit_cursor++];
        std::uint8_t bit = static_cast<std::uint8_t>(bmp.image_bytes[byte_idx]) & 1;
        msg_length |= (static_cast<std::uint32_t>(bit) << i);
    }

    
    if (32 + (static_cast<std::size_t>(msg_length) * 8) > pixel_indices.size())
    {
        throw std::runtime_error("Donnees corrompues ou aucun message cache trouve.");
    }

    
    std::string message;
    message.reserve(msg_length);

    for (std::uint32_t c_idx = 0; c_idx < msg_length; ++c_idx)
    {
        char c = 0;
        for (int i = 0; i < 8; ++i)
        {
            std::size_t byte_idx = pixel_indices[bit_cursor++];
            std::uint8_t bit = static_cast<std::uint8_t>(bmp.image_bytes[byte_idx]) & 1;
            c |= (bit << i);
        }
        message.push_back(c);
    }

    return message;
}

