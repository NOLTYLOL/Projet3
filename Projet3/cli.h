#pragma once
#include <string>

struct CliArgs
{
    std::string command;    
    std::string source_path;
    std::string dest_path;   
    std::string message;     
    std::string key;        
};

void print_help();

CliArgs parse_arguments(int argc, char* argv[]);