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