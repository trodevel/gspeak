/*

Simple Text to Speech interface

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

// $Id: i_text_to_speech.h 477 2014-04-29 17:50:50Z serge $


#ifndef I_TEXT_TO_SPEECH_H
#define I_TEXT_TO_SPEECH_H

#include <string>                   // std::string
#include "../utils/types.h"         // uint32

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

NAMESPACE_GSPEAK_START

class ITextToSpeech
{
public:

    enum lang_e
    {
        UNKNOWN     = 0,
        EN,
        DE,
        RU
    };

public:
    virtual ~ITextToSpeech() {};

    virtual bool say( const std::string & text, const std::string & filename, lang_e lang ) = 0;
    virtual std::string get_error_msg() const                                               = 0;
};

NAMESPACE_GSPEAK_END

#endif  // I_TEXT_TO_SPEECH_H
