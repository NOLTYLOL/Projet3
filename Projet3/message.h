#pragma once
#include "bmploader.h"
#include <cstdint>
#include <string>

std::string apply_xor(const std::string& text, const std::string& key);

void hide_byte_at(BmpImage& bmp, std::size_t byte_pos, std::uint8_t octet);

std::uint8_t extract_byte_at(const BmpImage& bmp, std::size_t byte_pos);

void encode_message(BmpImage& bmp, const std::string& message, const std::string& key);

std::string decode_message(const BmpImage& bmp, const std::string& key);