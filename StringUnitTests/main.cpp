#include <iostream>
#include <String/String.h>

void PrintStringInfo(const String& string);

int main()
{
    //CONSTRUCTOR TESTS START /////////////////////////////////////////////
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
    String move1c(std::move(copy1c));

    std::cout << "Move constructor\n";
    String move2c(std::move(copy2c));

    std::cout << "Move assignment\n";
    String move1a = std::move(move1c);

    std::cout << "Move assignment\n";
    String move2a = std::move(move2c);

    //CONSTRUCTOR TESTS END ///////////////////////////////////////////////


    // APPEND TEST START //////////////////////////////////////////////////
    String append1 = "Add";
    String append2 = "Add more";
    String append3 = "Add more to me. I need more";

    const char* more1 = "More";
    String more2 = "This is more.";
    String more3 = "This is definitely more.";

    append1 += more1;
    PrintStringInfo(append1);

    append2 += more2;
    PrintStringInfo(append2);

    append3 += more3;
    PrintStringInfo(append3);

    // APPEND TEST END ////////////////////////////////////////////////////
}

void PrintStringInfo(const String& string)
{
    std::cout << "Contents: " << string.CStr() << '\n' << "Size: " << string.Size() << '\n' << "Capacity: " << string.Capacity() << '\n' << "Storage: " << (string.Capacity() == 15 ? "Stack" : "Heap") << "\n\n";
}