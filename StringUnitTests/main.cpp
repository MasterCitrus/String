#include "CString.h"
#include <iostream>
#include <vector>

void PrintStringInfo(const String& string);

int main()
{
    //CONSTRUCTOR TESTS START /////////////////////////////////////////////

    {
        std::cout << "\n//////////////////////";
        std::cout << "\n  CONSTRUCTOR TESTS";
        std::cout << "\n//////////////////////\n\n";

        String string1;
        PrintStringInfo(string1);
        
        String stack = "Stack string.";
        PrintStringInfo(stack);
        
        String heap = "This is a heap string.";
        PrintStringInfo(heap);
        
        String copy1c(stack);
        PrintStringInfo(copy1c);

        String copy2c(heap);
        PrintStringInfo(copy2c);

        String copy1a = stack;
        PrintStringInfo(copy1a);

        String copy2a = heap;
        PrintStringInfo(copy2a);

        String move1c(std::move(copy1c));
        PrintStringInfo(move1c);

        String move2c(std::move(copy2c));
        PrintStringInfo(move2c);

        String move1a = std::move(move1c);
        PrintStringInfo(move1a);

        String move2a = std::move(move2c);
        PrintStringInfo(move2a);
    }

    //CONSTRUCTOR TESTS END ///////////////////////////////////////////////


    // APPEND TEST START //////////////////////////////////////////////////

    {
        std::cout << "\n//////////////////////";
        std::cout << "\n  APPEND TESTS";
        std::cout << "\n//////////////////////\n\n";

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
        std::cout << "\n//////////////////////";
        std::cout << "\n  CLEAR TESTS";
        std::cout << "\n//////////////////////\n\n";

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
        std::cout << "\n//////////////////////";
        std::cout << "\n  INSERT TESTS";
        std::cout << "\n//////////////////////\n\n";

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
        std::cout << "\n//////////////////////";
        std::cout << "\n  ERASE TESTS";
        std::cout << "\n//////////////////////\n\n";

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
        std::cout << "\n//////////////////////";
        std::cout << "\n  FIND TESTS";
        std::cout << "\n//////////////////////\n\n";

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



    // REPLACE TEST START /////////////////////////////////////////////////

    {
        std::cout << "\n//////////////////////";
        std::cout << "\n  REPLACE TESTS";
        std::cout << "\n//////////////////////\n\n";

        String replace1 = "Hi, {name}";
        PrintStringInfo(replace1);

        String replace2 = "Hello, {name}";
        PrintStringInfo(replace2);

        String replace3 = "Hello {name}, how are you?";
        PrintStringInfo(replace3);

        String name1 = "Mark";
        PrintStringInfo(name1);

        String name2 = "Catherine";
        PrintStringInfo(name2);

        replace1.Replace(replace1.Find("{name}"), 6, name1.CStr());
        PrintStringInfo(replace1);

        replace2.Replace(replace2.Find("{name}"), 6, name2.CStr());
        PrintStringInfo(replace2);

        replace3.Replace(replace3.Find("{name}"), 6, name2.CStr());
        PrintStringInfo(replace3);
    }

    // REPLACE TEST END ///////////////////////////////////////////////////

    

    // COMPARISON TEST START //////////////////////////////////////////////

    {
        std::cout << "\n//////////////////////";
        std::cout << "\n  COMPARSION TESTS";
        std::cout << "\n//////////////////////\n\n";

        std::vector<String> strings;

        String string1 = "String";
        String string2 = "String";
        String string3 = "String2";
        String string4 = "Strin";
        String string5 = "Strink";

        strings.push_back( string1 );
        strings.push_back( string2 );
        strings.push_back( string3 );
        strings.push_back( string4 );
        strings.push_back( string5 );

        for (int i = 0; i < strings.size(); i++)
        {

            for (int j = 0; j < strings.size(); j++)
            {
                auto str1 = strings[i];
                auto str2 = strings[j];

                std::cout << "Outer loop: " << i << '\n' << "Inner loop " << j << "\n\n";

                PrintStringInfo( str1 );

                // Equal
                str1 == str2 ? std::cout << "String 1 [ " << str1 << " ] is equal to String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is not equal to String 2 [ " << str2 << " ]" << '\n';

                // Not equal
                str1 != str2 ? std::cout << "String 1 [ " << str1 << " ] is not equal to String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is equal to String 2 [ " << str2 << " ]" << '\n';

                // Less than
                str1 < str2 ? std::cout << "String 1 [ " << str1 << " ] is less than String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is not less than String 2 [ " << str2 << " ]" << '\n';

                // Greater than
                str1 > str2 ? std::cout << "String 1 [ " << str1 << " ] is greater than String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is not greater than String 2 [ " << str2 << " ]" << '\n';

                //Less or equal to
                str1 <= str2 ? std::cout << "String 1 [ " << str1 << " ] is less than or equal to String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is not less than or equal to String 2 [ " << str2 << " ]" << '\n';

                //Greater or equal to
                str1 >= str2 ? std::cout << "String 1 [ " << str1 << " ] is greater than or equal to String 2 [ " << str2 << " ]" << '\n'
                    : std::cout << "String 1 [ " << str1 << " ] is not greater than or equal to String 2 [ " << str2 << " ]" << '\n';

                std::cout << '\n';
            }
        }
        
    }

    // COMPARISON TEST END ////////////////////////////////////////////////
}

void PrintStringInfo(const String& string)
{
    std::cout << "Contents: " << string.CStr() << '\n' << "Size: " << string.Size() << '\n' << "Capacity: " << string.Capacity() << '\n' << "Storage: " << (string.Capacity() == 15 ? "Stack" : "Heap") << "\n\n";
}