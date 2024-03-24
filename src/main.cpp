#include <iostream>
#include <string.h>

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
    InputHandler()
    {
        inputBuffer = new InputBuffer();
        inputBuffer->buf = NULL;
        inputBuffer->bufLength = 0;
        inputBuffer->inputLength = 0;
    }

    ~InputHandler()
    {
        delete inputBuffer->buf;
        delete inputBuffer;
    }

    void readInput()
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

    char *getBuf() const
    {
        return this->inputBuffer->buf;
    }
};

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
