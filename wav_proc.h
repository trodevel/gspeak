/*

Low-level WAV files processing.

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

// $Revision: 1573 $ $Date:: 2015-03-12 #$ $Author: serge $

#ifndef WAV_PROC_H
#define WAV_PROC_H

#include <string>                   // std::string
#include <boost/thread.hpp>         // boost::mutex

#include "namespace_lib.h"  // NAMESPACE_GSPEAK_START


NAMESPACE_GSPEAK_START

bool convert_mp3_to_wav( const std::string & inp, const std::string & outp );
bool join_wav_files( const std::vector< std::string > & inp, const std::string & outp );
bool append_wav_file( const std::string & base, const std::string & addition );

NAMESPACE_GSPEAK_END

#endif  // WAV_PROC_H
