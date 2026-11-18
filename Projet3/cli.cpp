#include "cli.h"

#include <iostream>
#include <stdexcept>

void print_help()
{
    std::cout << "Examples:\n";
    std::cout << "  projet3.exe copy /path/to/apple.bmp /path/to/copy.bmp\n";
    std::cout << "  projet3.exe encode /path/to/apple.bmp /path/to/secret.bmp \"Secret Message\"\n";
    std::cout << "  projet3.exe encode /path/to/apple.bmp /path/to/secret.bmp \"Super Secret Message\" \"my secret key\"\n";
    std::cout << "  projet3.exe decode /path/to/secret.bmp\n";
    std::cout << "  projet3.exe decode /path/to/secret.bmp \"my secret key\"\n";
}

CliArgs parse_arguments(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_help();
        throw std::runtime_error("Missing command");
    }

    CliArgs args;
    args.command = argv[1];

    if (args.command == "help")
    {
        print_help();
        return args;
    }
    else if (args.command == "copy")
    {
        if (argc != 3 && argc != 4)
        {
            print_help();
            throw std::runtime_error("Invalid copy arguments");
        }

        args.source_path = argv[2];
        if (argc == 4)
        {
            args.dest_path = argv[3];
        }
        else
        {
            args.dest_path = "copy.bmp";
        }

        if (args.source_path == args.dest_path)
        {
            throw std::runtime_error("Source and destination files cannot be the same");
        }
    }
    else if (args.command == "encode")
    {

        if (argc != 5 && argc != 6)
        {
            print_help();
            throw std::runtime_error("Invalid encode arguments");
        }

        args.source_path = argv[2];
        args.dest_path = argv[3];
        args.message = argv[4];

        if (args.source_path == args.dest_path)
        {
            throw std::runtime_error("Source and destination files cannot be the same");
        }

        if (argc == 6)
        {
            args.key = argv[5];
            if (args.key.empty())
            {
                throw std::runtime_error("Key cannot be empty");
            }
        }
    }
    else if (args.command == "decode")
    {

        if (argc != 3 && argc != 4)
        {
            print_help();
            throw std::runtime_error("Invalid decode arguments");
        }

        args.source_path = argv[2];

        if (argc == 4)
        {
            args.key = argv[3];
            if (args.key.empty())
            {
                throw std::runtime_error("Key cannot be empty");
            }
        }
    }
    else
    {
        print_help();
        throw std::runtime_error("Unknown command: " + args.command);
    }

    return args;
}