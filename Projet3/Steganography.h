#pragma once


#include "bmploader.h"
#include <string>


void hide_message(BmpImage& bmp, const std::string& message);


std::string extract_message(const BmpImage& bmp);
