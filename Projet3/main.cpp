#include "bmploader.h"
#include "cli.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        CliArgs args = parse_arguments(argc, argv);

        if (args.command == "copy")
        {
            copy_bmp(args.source_path, args.dest_path);
            std::cout << "Copied " << args.source_path << " to " << args.dest_path << " successfully!" << std::endl;
        }
        else if (args.command == "encode")
        {
            std::cout << "Command : " << args.command << std::endl;
            std::cout << "Source  : " << args.source_path << std::endl;
            std::cout << "Dest    : " << args.dest_path << std::endl;
            std::cout << "Message : " << args.message << std::endl;
            std::cout << "Key     : " << args.key << std::endl;

            BmpImage bmp = load_bmp(args.source_path);
            std::cout << "Loaded BMP (" << bmp.width << " x " << bmp.height << ")" << std::endl;

            // Faudra la fonction encode ici

        std::cout << "Loaded image.bmp (" << bmp.width << " x " << bmp.height << ")" << std::endl;

        std::cout << "Pixel offset: " << bmp.pixel_offset << std::endl;
        std::cout << "Row stride  : " << bmp.stride << std::endl;
            save_bmp(args.dest_path, bmp);
            std::cout << "Saved " << args.dest_path << " successfully!" << std::endl;
        }
        else if (args.command == "decode")
        {
            std::cout << "Command : " << args.command << std::endl;
            std::cout << "Source  : " << args.source_path << std::endl;
            std::cout << "Key     : " << args.key << std::endl;
            BmpImage bmp = load_bmp(args.source_path);
            std::cout << "Loaded BMP (" << bmp.width << " x " << bmp.height << ")" << std::endl;

        constexpr unsigned char valeur_a_cacher = 0xA5; // [teste -->
        constexpr int x = 0;
        constexpr int y = 0;

        hide_octet(bmp, x, y, valeur_a_cacher);

        const unsigned char valeur_extraite =
            extraire_octet(bmp, x, y);

        if (valeur_extraite != valeur_a_cacher)
        {
            throw std::runtime_error(
                "Error : The extracted byte differs from the cached byte.");
        }

        std::cout << "Byte hide and extraction work correctly : "
			<< static_cast<int>(valeur_extraite) << std::endl; // <-- test]
        save_bmp("output/copy.bmp", bmp);
        std::cout << "Saved copy.bmp successfully!" << std::endl;

            // Faudra la fonction decode ici
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}