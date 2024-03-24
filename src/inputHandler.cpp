#include "../include/inputHandler.hpp"

InputHandler::InputHandler()
{
    inputBuffer = new InputBuffer();
    inputBuffer->buf = NULL;
    inputBuffer->bufLength = 0;
    inputBuffer->inputLength = 0;
}

InputHandler ::~InputHandler()
{
    delete inputBuffer->buf;
    delete inputBuffer;
}

void InputHandler::readInput()
{
    ssize_t bytesRead = getline(&(inputBuffer->buf), &(inputBuffer->bufLength), stdin);

    if (bytesRead <= 0)
    {
        std::cerr << "Error reading input\n";
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    inputBuffer->inputLength = bytesRead - 1;
    inputBuffer->buf[bytesRead - 1] = '\0';
}

char *InputHandler::getBuf() const
{
    return this->inputBuffer->buf;
}
