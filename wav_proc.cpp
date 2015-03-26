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

// $Revision: 1647 $ $Date:: 2015-03-25 #$ $Author: serge $


#include "wav_proc.h"               // self

#include <sstream>                  // std::ostringstream

#include "../utils/dummy_logger.h"  // dummy_log
#include "../wave/wave.h"           // wave::Wave
#include "../convimp3/codec.h"      // convimp3::decode

#include "namespace_lib.h"          // NAMESPACE_GSPEAK_START

#define MODULENAME      "wav_proc"

NAMESPACE_GSPEAK_START

bool convert_mp3_to_wav( const std::string & inp, const std::string & outp )
{
    try
    {
        bool b = convimp3::Codec::decode( inp.c_str(), outp.c_str() );

        if( b == false )
        {
            dummy_log_error( MODULENAME, "cannot convert %s into %s", inp.c_str(), outp.c_str() );

            return false;
        }
    }
    catch( std::exception & e )
    {
        dummy_log_error( MODULENAME, "exception: cannot convert %s into %s, %s", inp.c_str(), outp.c_str(), e.what() );

        return false;
    }

    return true;
}

bool join_wav_files( const std::vector< std::string > & inp, const std::string & outp )
{
    if( inp.empty() )
        return false;

    std::ostringstream s;

    try
    {
        wave::Wave res;

        for( auto & str : inp )
        {
            s << str << " ";

            res += str;
        }

        res.save( outp );

        dummy_log_debug( MODULENAME, "joined %s into %s", s.str().c_str(), outp.c_str() );
    }
    catch( std::exception &e )
    {
        dummy_log_error( MODULENAME, "cannot join WAV files: %s", s.str().c_str() );
        return false;
    }

    return true;
}

NAMESPACE_GSPEAK_END
