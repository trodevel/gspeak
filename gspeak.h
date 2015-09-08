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

// $Revision: 2532 $ $Date:: 2015-09-09 #$ $Author: serge $

#ifndef GSPEAK_GSPEAK_H
#define GSPEAK_GSPEAK_H

#include "i_text_to_speech.h"       // IGtts

namespace gspeak
{

class GSpeak: virtual public ITextToSpeech
{
public:

public:
    GSpeak();

    ~GSpeak();

    virtual bool say( const std::string & text, const std::string & filename, const std::string & lang, std::string & error );

private:

    static const std::string escape_string( const std::string & s );
    static const std::string prepare_url( const std::string & text, const std::string & lang );

};

} // namespace gspeak

#endif  // GSPEAK_GSPEAK_H
