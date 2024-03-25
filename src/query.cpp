#include "../include/query.hpp"

Query::Query(InputHandler &inputHandler) : inputHandler(inputHandler) {}

QueryStatus Query::preProcessQuery()
{
    if (strncasecmp(inputHandler.getBuf(), "insert ", 7) == 0)
    {
        type = Q_INSERT;
        return QUERY_SUCCESS;
    }
    if (strncasecmp(inputHandler.getBuf(), "select ", 7) == 0)
    {
        type = Q_SELECT;
        return QUERY_SUCCESS;
    }

    return QUERY_UNRECOGNIZED;
}

void Query::execute()
{
    switch (type)
    {
    case (Q_INSERT):
        std ::cout << "This is where we would do an insert.\n";
        break;
    case (Q_SELECT):
        std ::cout << "This is where we would do a select.\n";
        break;
    }
}

MetaCommandStatus Query::processMetaCommand()
{
    if (strcmp(inputHandler.getBuf(), ".exit") == 0)
    {
        std::cout << "Exiting from DB.\nThank you for using.\n";
        exit(EXIT_SUCCESS);
    }
    else
    {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
    return META_COMMAND_SUCCESS;
}
