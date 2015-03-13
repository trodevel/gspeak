#include <cstdio>
#include <iostream>         // cout

#include "gspeak.h"         // GSpeak

#include "../utils/dummy_logger.h"          // dummy_log_set_log_level

int main()
{
    printf( "Hello, world\n" );

    dummy_logger::set_log_level( log_levels_log4j::DEBUG );

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

    g.save_state();

    return 0;
}
