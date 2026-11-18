#include "bmploader.h"
#include "cli.h"
#include "message.h"
#include <iostream>

int main(int argc, char* argv[])
{
    CliArgs args = parse_arguments(argc, argv);

    if (args.command == "copy")
    {
        copy_bmp(args.source_path, args.dest_path);
        std::cout << "Copied " << args.source_path << " to " << args.dest_path << " successfully!" << std::endl;
    }
    else if (args.command == "encode")
    {
        BmpImage bmp = load_bmp(args.source_path);
        std::cout << "Loaded BMP (" << bmp.width << " x " << bmp.height << ")" << std::endl;

        encode_message(bmp, args.message, args.key);

        save_bmp(args.dest_path, bmp);
        std::cout << "Message encoded into " << args.dest_path << " successfully!" << std::endl;
    }
    else if (args.command == "decode")
    {
        BmpImage bmp = load_bmp(args.source_path);
        std::cout << "Loaded BMP (" << bmp.width << " x " << bmp.height << ")" << std::endl;

        std::string secret = decode_message(bmp, args.key);
        std::cout << "Decoded message: " << secret << std::endl;
    }

    return 0;
}