#include "log.hpp"

void logFatal(std::string input)
{
    std::cout << "FATAL: " << input << std::endl;

    exit(1);
}

void logWarning(std::string input)
{
    std::cout << "WARN: " << input << std::endl;
}

void logVerbose(std::string input)
{
    std::cout << "LOG: " << input << std::endl;
}