#include <iostream>
#include <string.h>
#include "../include/inputHandler.hpp"

void prompt()
{
    std::cout << "db > ";
}

int main(int argc, char const *argv[])
{
    InputHandler inputHandler;
    while (true)
    {
        prompt();
        inputHandler.readInput();

        if (strcmp(inputHandler.getBuf(), ".exit") == 0)
        {
            std::cout << "Exiting from DB.\nThank you for using.\n";
            exit(EXIT_SUCCESS);
        }
        else
        {
            std ::cout << "Unrecognized command" << inputHandler.getBuf() << std::endl;
        }
    }
    return 0;
}
