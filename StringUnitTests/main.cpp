#include <iostream>
#include <String/String.h>

int main()
{
    std::cout << "Default constructed\n";
    String string1;

    std::cout << "Const char constructor\n";
    String stack = "Stack string.";

    std::cout << "Const char constructor\n";
    String heap = "This is a heap string.";

    std::cout << "Copy constructor\n";
    String copy1c(stack);

    std::cout << "Copy constructor\n";
    String copy2c(heap);

    std::cout << "Copy assignment\n";
    String copy1a = stack;

    std::cout << "Copy assignment\n";
    String copy2a = heap;

    std::cout << "Move constructor\n";
    String move1c(std::move(stack));

    std::cout << "Move constructor\n";
    String move2c(std::move(heap));

    std::cout << "Move assignment\n";
    String move1a = std::move(move1c);

    std::cout << "Move assignment\n";
    String move2a = std::move(move2c);
}
