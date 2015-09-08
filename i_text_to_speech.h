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

// $Revision: 2526 $ $Date:: 2015-09-08 #$ $Author: serge $


#ifndef I_TEXT_TO_SPEECH_H
#define I_TEXT_TO_SPEECH_H

#include <string>               // std::string

namespace gspeak
{

class ITextToSpeech
{
public:
    virtual ~ITextToSpeech() {};

    virtual bool say( const std::string & text, const std::string & filename, const std::string & lang, std::string & error ) = 0;
};

} // namespace gspeak

#endif  // I_TEXT_TO_SPEECH_H
