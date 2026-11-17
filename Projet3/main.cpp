#include "bmploader.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>

int main()
{


  
        std::cout
            << "Working Directory : "
            << std::filesystem::current_path()
            << '\n';
   

    try
    {
        BmpImage bmp = load_bmp("input/image.bmp");

        std::cout << "Loaded image.bmp (" << bmp.width << " x " << bmp.height << ")" << std::endl;
        std::cout << "Pixel offset: " << bmp.pixel_offset << std::endl;
        std::cout << "Row stride  : " << bmp.stride << std::endl;

        save_bmp("output/copy.bmp", bmp);
        std::cout << "Saved copy.bmp successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}