/*

Simple wrapper over Google Translate TTS engine

Copyright (C) 2014 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Id: gtts.cpp 388 2014-04-15 17:18:36Z serge $


#include "gtts.h"           // self

#include <sstream>                  // std::ostringstream
#include <cstdlib>                  // system
#include <boost/algorithm/string/replace.hpp>   // replace_all_copy

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/wrap_mutex.h"        // SCOPE_LOCK

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "Gtts"

NAMESPACE_GSPEAK_START

Gtts::Gtts()
{
}

Gtts::~Gtts()
{
}

const std::string Gtts::escape_string( const std::string & s )
{
    std::string res = boost::replace_all_copy<std::string>( s, " ", "+" );

    boost::replace_all( res, "\"", "\\\"" );

    return res;
}

bool Gtts::say( const std::string & text, const std::string & filename, lang_e lang )
{
    std::string text_no_sp = escape_string( text );

    std::ostringstream s;

    const std::string & lang_s  = to_string( lang );

    s <<
        "#/bin/bash \n" <<
        "wget -q -U Mozilla -O " << filename << " \"http://translate.google.com/translate_tts?ie=UTF-8&tl=" << lang_s << "&q=" << text_no_sp << "\"\n";

    std::cout << "executing: " << s.str() << std::endl;

    system( s.str().c_str() );
    return false;
}

const std::string & Gtts::to_string( lang_e l )
{
    static std::string en( "en" );
    static std::string de( "de" );
    static std::string ru( "ru" );

    switch( l )
    {
    case EN:
        return en;
    case DE:
        return de;
    case RU:
        return ru;
    default:
        break;
    }

    return en;
}

std::string Gtts::get_error_msg() const
{
    return error_msg_;
}

NAMESPACE_GSPEAK_END
