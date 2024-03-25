#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>

struct InputBuffer
{
    char *buf;
    size_t bufLength;
    size_t inputLength;
};

class InputHandler
{
private:
    InputBuffer *inputBuffer;

public:
    InputHandler();

    ~InputHandler();

    void readInput();

    char *getBuf() const;
};

#endif