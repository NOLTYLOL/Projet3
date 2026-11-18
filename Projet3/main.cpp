#include "bmploader.h"
#include "steganography.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        
        BmpImage bmp = load_bmp("input/image.bmp");
        std::cout << "Loaded image.bmp (" << bmp.width << " x " << bmp.height << ")" << std::endl;

       
        std::string secret = "Salut !";
        hide_message(bmp, secret);
        std::cout << "Le message a ete cache avec succes." << std::endl;

        std::filesystem::create_directories("output");
        std::filesystem::remove("output/secret.bmp");
        save_bmp("output/secret.bmp", bmp);
        std::cout << "Le fichier secret.bmp a ete enregistre avec succes!" << std::endl;

        
        BmpImage loaded_secret_bmp = load_bmp("output/secret.bmp");
        std::string extracted = extract_message(loaded_secret_bmp);
        std::cout << "Extracted message: " << extracted << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
