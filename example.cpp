#include <iostream>         // cout

#include "gspeak.h"         // GSpeak

int main()
{
    std::string lang;

    std::cout << "enter language (en, de, ru): ";
    std::getline( std::cin, lang );

    std::string input;

    std::cout << "enter text: ";
    std::getline( std::cin, input );

    gspeak::GSpeak g;

    std::string filename( "text.mp3" );
    std::string error;

    bool b = g.say( input, filename, lang, error );

    if( b == false )
        std::cout << "cannot download '" << filename << "', " << error << std::endl;
    else
        std::cout << "downloaded: OK '" << filename << "'" << std::endl;

    return 0;
}
