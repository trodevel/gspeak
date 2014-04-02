#include <cstdio>
#include <iostream>         // cout

#include "gspeak.h"         // GSpeak

int main()
{
    printf( "Hello, world\n" );

    std::string input;

    std::cout << "enter text: ";
    std::getline( std::cin, input );

    gspeak::Config config;

    config.word_base_path   = "voc/words.txt";
    config.data_path        = "voc";
    config.temp_path        = "voc_temp";

    gspeak::GSpeak g;

    bool b = g.init( config );

    if( b == false )
    {
        std::cout << "ERROR: cannot initialize GSpeak" << std::endl;

        return 0;
    }

    g.say( input, "text.wav" );

    return 0;
}
