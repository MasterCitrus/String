#include <iostream>
#include <String/String.h>

void PrintStringInfo(const String& string);

int main()
{
    //CONSTRUCTOR TESTS START /////////////////////////////////////////////

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
        String move1c(std::move(copy1c));

        std::cout << "Move constructor\n";
        String move2c(std::move(copy2c));

        std::cout << "Move assignment\n";
        String move1a = std::move(move1c);

        std::cout << "Move assignment\n";
        String move2a = std::move(move2c);
    }

    //CONSTRUCTOR TESTS END ///////////////////////////////////////////////


    // APPEND TEST START //////////////////////////////////////////////////

    {
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

        String string1 = "This";
        String string2 = "More";

        String string3 = "Gallifrey";
        String string4 = " Falls No More";

        String string5 = "I have brought Peace, Freedom and Prosperity";
        String string6 = " to my new empire";

        String result1 = string1 + string2;
        PrintStringInfo( result1 );

        String result2 = string3 + string4;

        String result3 = string5 + string6;
    }

    // APPEND TEST END ////////////////////////////////////////////////////



    // CLEAR TEST START ///////////////////////////////////////////////////

    {
        String clearMe1 = "Clear me.";
        String clearMe2 = "I need to be cleared.";

        clearMe1.Clear();
        PrintStringInfo(clearMe1);

        clearMe2.Clear();
        PrintStringInfo(clearMe2);
    }

    // CLEAR TEST END /////////////////////////////////////////////////////



    // INSERT TEST START //////////////////////////////////////////////////

    {
        String string1 = "SE";
        String string2 = "Start End";
        String string3 = "I start here. I end here.";

        const char* insert1 = "M";
        String insert2 = "Middle ";
        String insert3 = "I am the middle. ";

        string1.Insert(1, insert1);
        PrintStringInfo(string1);

        string2.Insert(6, insert2);
        PrintStringInfo(string2);

        string3.Insert(14, insert3);
        PrintStringInfo(string3);
    }

    // INSERT TEST END ////////////////////////////////////////////////////


    // ERASE TEST START ///////////////////////////////////////////////////

    {
        String erase1 = "Keep. Erase.";
        PrintStringInfo( erase1 );

        String erase2 = "Keep me. Erase me.";
        PrintStringInfo( erase2 );

        String erase3 = "Keep me. ERASE ME. Keep me.";

        erase1.Erase( 5, 7 );
        PrintStringInfo( erase1 );

        erase2.Erase( 8, 10 );
        PrintStringInfo( erase2 );

        erase3.Erase( 8, 10 );
        PrintStringInfo( erase3 );
    }

    // ERASE TEST END /////////////////////////////////////////////////////


    // FIND TEST START ////////////////////////////////////////////////////

    {
        String haystack1 = "WooFINDooo";
        PrintStringInfo( haystack1 );

        String haystack2 = "Can you FIND ME in here?";
        PrintStringInfo( haystack2 );

        String needle1 = "FIND ME";

        size_t found1 = haystack1.Find( "FIND" );

        size_t found2 = haystack2.Find( needle1 );

        size_t found3 = haystack1.RFind( "FIND" );

        size_t found4 = haystack2.RFind( needle1 );

        if (found1 != String::npos)
        {
            std::cout << "FIND found at index " << found1 << '\n';
        }
        else
        {
            std::cout << "FIND not found. NPOS = " << found1 << '\n';
        }

        if (found2 != String::npos)
        {
            std::cout << needle1.CStr() << " found at index " << found2 << '\n';
        }
        else
        {
            std::cout << needle1.CStr() << " not found. NPOS = " << found2 << '\n';
        }

        if (found3 != String::npos)
        {
            std::cout << "FIND found at index " << found3 << '\n';
        }
        else
        {
            std::cout << "FIND not found. NPOS = " << found3 << '\n';
        }

        if (found4 != String::npos)
        {
            std::cout << needle1.CStr() << " found at index " << found4 << '\n';
        }
        else
        {
            std::cout << needle1.CStr() << " not found. NPOS = " << found4 << '\n';
        }

    }

    // FIND TEST END //////////////////////////////////////////////////////
}

void PrintStringInfo(const String& string)
{
    std::cout << "Contents: " << string.CStr() << '\n' << "Size: " << string.Size() << '\n' << "Capacity: " << string.Capacity() << '\n' << "Storage: " << (string.Capacity() == 15 ? "Stack" : "Heap") << "\n\n";
}