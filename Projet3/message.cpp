#include "message.h"
#include <stdexcept>

std::string apply_xor(const std::string& text, const std::string& key)
{
    if (key.empty())
    {
        return text;
    }

    std::string result = text;

    for (std::size_t i = 0; i < result.size(); ++i)
    {
        std::uint8_t msg_byte = static_cast<std::uint8_t>(result[i]);
        std::uint8_t key_byte = static_cast<std::uint8_t>(key[i % key.size()]);

        result[i] = static_cast<char>(msg_byte ^ key_byte);
    }

    return result;
}

void hide_byte_at(BmpImage& bmp, std::size_t byte_pos, std::uint8_t octet)
{
    for (int bit_index = 0; bit_index < 8; ++bit_index)
    {
        std::size_t bit_pos = byte_pos * 8 + bit_index;
        std::size_t pixel_index = bit_pos / 3;
        std::size_t channel = bit_pos % 3;
        std::size_t row = pixel_index / bmp.width;
        std::size_t column = pixel_index % bmp.width;

        std::size_t index =
            bmp.pixel_offset +
            row * bmp.stride +
            column * 3 +
            channel;

        std::uint8_t component = static_cast<std::uint8_t>(bmp.image_bytes[index]);
        std::uint8_t bit = static_cast<std::uint8_t>((octet >> (7 - bit_index)) & 1u);

        component = static_cast<std::uint8_t>((component & 0xFEu) | bit);
        bmp.image_bytes[index] = static_cast<char>(component);
    }
}

std::uint8_t extract_byte_at(const BmpImage& bmp, std::size_t byte_pos)
{
    std::uint8_t octet = 0;

    for (int bit_index = 0; bit_index < 8; ++bit_index)
    {
        std::size_t bit_pos = byte_pos * 8 + bit_index;
        std::size_t pixel_index = bit_pos / 3;
        std::size_t channel = bit_pos % 3;
        std::size_t row = pixel_index / bmp.width;
        std::size_t column = pixel_index % bmp.width;

        std::size_t index =
            bmp.pixel_offset +
            row * bmp.stride +
            column * 3 +
            channel;

        std::uint8_t component = static_cast<std::uint8_t>(bmp.image_bytes[index]);
        std::uint8_t bit = component & 1u;

        octet = static_cast<std::uint8_t>((octet << 1) | bit);
    }

    return octet;
}

void encode_message(BmpImage& bmp, const std::string& message, const std::string& key)
{
    std::size_t max_bits = static_cast<std::size_t>(bmp.width) * bmp.height * 3;
    std::size_t max_bytes = max_bits / 8;

    std::size_t total_bytes = 7 + message.size();

    if (total_bytes > max_bytes)
    {
        throw std::runtime_error("Message is too long for this BMP image");
    }

    std::string data_to_hide = apply_xor(message, key);

    hide_byte_at(bmp, 0, static_cast<std::uint8_t>('S'));
    hide_byte_at(bmp, 1, static_cast<std::uint8_t>('T'));

    std::uint8_t flag = 0;
    if (!key.empty())
    {
        flag = 1;
    }
    hide_byte_at(bmp, 2, flag);

    std::size_t msg_len = data_to_hide.size();
    hide_byte_at(bmp, 3, static_cast<std::uint8_t>(msg_len & 0xFFu));
    hide_byte_at(bmp, 4, static_cast<std::uint8_t>((msg_len >> 8) & 0xFFu));
    hide_byte_at(bmp, 5, static_cast<std::uint8_t>((msg_len >> 16) & 0xFFu));
    hide_byte_at(bmp, 6, static_cast<std::uint8_t>((msg_len >> 24) & 0xFFu));

    for (std::size_t i = 0; i < data_to_hide.size(); ++i)
    {
        hide_byte_at(bmp, 7 + i, static_cast<std::uint8_t>(data_to_hide[i]));
    }
}

std::string decode_message(const BmpImage& bmp, const std::string& key)
{
    std::size_t max_bits = static_cast<std::size_t>(bmp.width) * bmp.height * 3;
    std::size_t max_bytes = max_bits / 8;

    if (max_bytes < 7)
    {
        throw std::runtime_error("BMP image is too small to contain a message");
    }

    char sig0 = static_cast<char>(extract_byte_at(bmp, 0));
    char sig1 = static_cast<char>(extract_byte_at(bmp, 1));

    if (sig0 != 'S' || sig1 != 'T')
    {
        throw std::runtime_error("No hidden message found in this BMP");
    }

    std::uint8_t flag = extract_byte_at(bmp, 2);
    if (flag != 0 && flag != 1)
    {
        throw std::runtime_error("Corrupted message metadata");
    }

    if (flag == 1 && key.empty())
    {
        throw std::runtime_error("This message is encrypted: a key is required");
    }

    if (flag == 0 && !key.empty())
    {
        throw std::runtime_error("This message is not encrypted: do not provide a key");
    }

    std::uint8_t b0 = extract_byte_at(bmp, 3);
    std::uint8_t b1 = extract_byte_at(bmp, 4);
    std::uint8_t b2 = extract_byte_at(bmp, 5);
    std::uint8_t b3 = extract_byte_at(bmp, 6);

    std::size_t msg_len =
        static_cast<std::size_t>(b0) |
        (static_cast<std::size_t>(b1) << 8) |
        (static_cast<std::size_t>(b2) << 16) |
        (static_cast<std::size_t>(b3) << 24);

    if (7 + msg_len > max_bytes)
    {
        throw std::runtime_error("Invalid or corrupted message length in BMP");
    }

    std::string extracted = "";
    for (std::size_t i = 0; i < msg_len; ++i)
    {
        extracted += static_cast<char>(extract_byte_at(bmp, 7 + i));
    }

    if (flag == 1)
    {
        extracted = apply_xor(extracted, key);
    }

    return extracted;
}