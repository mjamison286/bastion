#include "include/io.hpp"

struct IOData
{
    std::string input;
    std::string output;
};

std::string processInput(int argc, char** argv, struct IOData* data)
{
    if(!argc)
    {
        exit(1);
    }

    for(int i = 0; i < argc; i++)
    {
        std::string arg = argv[i];
        if(i != argc - 1)
        {
            if(arg == "-i")
            {
                data->input = argv[i + 1];
            }
            else if(arg == "-o")
            {
                data->output = argv[i + 1];
            }
        }
    }

    if(data->input == "")
    {
        
    }

    if(data->output == "")
    {
        data->output = "./output";
    }

    return "";
}