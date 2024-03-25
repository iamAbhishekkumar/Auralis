#ifndef CONSTANTS_H
#define CONSTANTS_H
enum QueryStatus
{
    QUERY_SUCCESS,
    QUERY_UNRECOGNIZED
};

enum QueryType
{
    Q_INSERT,
    Q_SELECT
};

enum MetaCommandStatus
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
};
#endif