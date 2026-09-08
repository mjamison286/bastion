//header files here
#include "include/io.hpp"

//global variables
static std::string filePath;
static std::string outputPath;
//argc is the amount of args passed into the program, while the argv array are the actual arguments as c-strings

struct IOData
{
    std::string input;
    std::string output;
};

int main(int argc, char* argv[])
{
    struct IOData data;

    processInput(argc, argv, &data);

    filePath = data.input;
    outputPath = data.output;

    if(filePath != "")
    {
        std::cout << "filepath is: " << filePath << std::endl;
    }
    else
    {
        std::cout << "There is no filepath" << std::endl;
    }

    if(outputPath != "")
    {
        std::cout << "outputpath is: " << outputPath << std::endl;
    }
    else
    {
        std::cout << "there is no outputpath" << std::endl;
    }



    return 0;
}