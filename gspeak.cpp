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

// $Revision: 2565 $ $Date:: 2015-09-16 #$ $Author: serge $


#include "gspeak.h"                 // self

#include <sstream>                  // std::ostringstream
#include <boost/algorithm/string/replace.hpp>   // replace_all_copy

#include "../utils/HTTPDownloader.hpp"  // HTTPDownloader

using namespace gspeak;

GSpeak::GSpeak()
{
}

GSpeak::~GSpeak()
{
}

const std::string GSpeak::escape_string( const std::string & s )
{
    std::string res = boost::replace_all_copy<std::string>( s, " ", "+" );

    boost::replace_all( res, "\"", "\\\"" );

    return res;
}

const std::string GSpeak::prepare_url( const std::string & text, const std::string & lang )
{
    std::ostringstream s;

    s << "http://translate.google.com/translate_tts?ie=UTF-8&tl=" << lang << "&q=" << text << "&client=t";

    return s.str();
}

bool GSpeak::say( const std::string & text, const std::string & filename, const std::string & lang, std::string & error )
{
    std::string text_no_sp = escape_string( text );

    std::string url = prepare_url( text_no_sp, lang );

    HTTPDownloader h;

    static const std::string agent("Mozilla/5.0 (X11; Linux x86_64; rv:39.0) Gecko/20100101 Firefox/39.0)");

    bool b = h.download_file( url, filename, error, agent );

    return b;
}

