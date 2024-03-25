#ifndef QUERY_H
#define QUERY_H

#include "inputHandler.hpp"
#include "constants.hpp"
#include <string.h>

class Query
{
private:
    QueryType type;
    InputHandler &inputHandler;

public:
    Query(InputHandler &inputHandler);

    QueryStatus preProcessQuery();

    void execute();

    MetaCommandStatus processMetaCommand();
};

#endif