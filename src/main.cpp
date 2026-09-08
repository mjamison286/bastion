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

    std::cout << filePath << std::endl;



    return 0;
}