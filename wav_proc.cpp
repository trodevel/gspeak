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

// $Id: wav_proc.cpp 387 2014-04-15 16:45:05Z serge $


#include "wav_proc.h"           // self

#include <sstream>                  // std::ostringstream
#include <cstdlib>                  // system

#include "../utils/dummy_logger.h"      // dummy_log

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "wav_proc"

NAMESPACE_GSPEAK_START

bool convert_mp3_to_wav( const std::string & inp, const std::string & outp )
{
    std::ostringstream s;

    s <<
        "#/bin/bash \n" <<
        "sox -t mp3 " << inp << " -t wav " << outp;

    dummy_log( 0, MODULENAME, "DBG: executing: %s", s.str().c_str() );

    system( s.str().c_str() );

    return true;
}

bool join_wav_files( const std::vector< std::string > & inp, const std::string & outp )
{
    if( inp.empty() )
        return false;

    std::ostringstream s;

    if( inp.size() == 1 )
    {
        s <<
            "#/bin/bash \n"
            "cp " << inp[0] << " " << outp;
    }
    else
    {
        s <<
            "#/bin/bash \n"
            "sox ";

        for( auto str : inp )
        {
            s << str << " ";
        }

        s << outp;
    }

    dummy_log( 0, MODULENAME, "DBG: executing: %s", s.str().c_str() );

    system( s.str().c_str() );

    return true;
}

bool append_wav_file( const std::string & base, const std::string & addition )
{
    if( base.empty() )
        return false;

    if( addition.empty() )
        return false;

    std::string tmp = base + ".tmp.wav";

    std::ostringstream s;

    s <<
        "#/bin/bash \n"
        "sox " << base << " " << addition << " " << tmp << "\n"
        "mv " << tmp << " " << base;

    dummy_log( 0, MODULENAME, "DBG: executing: %s", s.str().c_str() );

    system( s.str().c_str() );

    return true;
}

NAMESPACE_GSPEAK_END
