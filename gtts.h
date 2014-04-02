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

// $Id: gtts.h 338 2014-04-01 22:43:01Z serge $

#ifndef GTTS_H
#define GTTS_H

#include <string>                   // std::string
#include <boost/thread.hpp>         // boost::mutex

#include "i_text_to_speech.h"       // IGtts

#include "namespace_gspeak.h"  // NAMESPACE_GSPEAK_START


NAMESPACE_GSPEAK_START

class Gtts: virtual public ITextToSpeech
{
public:

public:
    Gtts();

    ~Gtts();

    bool say( const std::string & text, const std::string & filename, lang_e lang = lang_e::EN );
    std::string get_error_msg() const;

private:

    static const std::string& to_string( lang_e l );

public:


private:
    mutable boost::mutex        mutex_;

    std::string                 error_msg_;
};

NAMESPACE_GSPEAK_END

#endif  // GTTS_H
