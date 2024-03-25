#include <iostream>
#include <string.h>
#include "../include/inputHandler.hpp"
#include "../include/constants.hpp"
#include "../include/query.hpp"

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
        Query query(inputHandler);

        if (inputHandler.getBuf()[0] == '.')
        {
            switch (query.processMetaCommand())
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                std ::cout << "Unrecognized command" << inputHandler.getBuf() << std::endl;
                continue;
            }
        }

        switch (query.preProcessQuery())
        {
        case (QUERY_SUCCESS):
            break;
        case (QUERY_UNRECOGNIZED):
            std::cout << "Unrecognized keyword at start of " << inputHandler.getBuf() << std::endl;
            continue;
        }

        query.execute();
        printf("Executed.\n");
    }

    return 0;
}
