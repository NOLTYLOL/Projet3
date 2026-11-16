#include "bmploader.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        BmpImage bmp = load_bmp("apple.bmp");

        std::cout << "Loaded apple.bmp (" << bmp.width << " x " << bmp.height << ")" << std::endl;
        std::cout << "Pixel offset: " << bmp.pixel_offset << std::endl;
        std::cout << "Row stride  : " << bmp.stride << std::endl;

        save_bmp("copy.bmp", bmp);
        std::cout << "Saved copy.bmp successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}