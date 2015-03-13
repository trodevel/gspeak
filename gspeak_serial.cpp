/*

Cache over Gtts. Serialization.

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

// $Revision: 1577 $ $Date:: 2015-03-12 #$ $Author: serge $


#include "gspeak.h"           // self

#include <sstream>                  // std::ostringstream
#include <fstream>                  // std::ofstream
#include <limits>                   // std::numeric_limits

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/trim.h"              // trim

#define MODULENAME      "GSpeak"

NAMESPACE_GSPEAK_START

std::ofstream & operator <<( std::ofstream & os, const GSpeak::Token & t )
{
    os << t.id << " " << static_cast<unsigned int>( t.lang );

    return os;
}

std::ofstream & operator <<( std::ofstream & os, const GSpeak::MapTokenToString::value_type & v )
{
    os << v.first << " " << v.second << std::endl;

    return os;
}

bool GSpeak::save_state__()
{
    std::ofstream ofs( config_.word_base_path );

    for( const auto & e : id_to_word_ )
    {
        ofs << e;
    }

    dummy_log_debug( MODULENAME, "saved words = %u", id_to_word_.size() );

    return true;
}

bool GSpeak::load_state__()
{
    std::ifstream in_file( config_.word_base_path.c_str() );
    std::string line;

    if( in_file.is_open() == false )
    {
        dummy_log_error( MODULENAME, "cannot open file %s", config_.word_base_path.c_str() );

        return false;
    }

    uint32 line_num = 0;

    while( in_file.good() )
    {
        std::getline( in_file, line );

        line_num++;

        bool b = parse_line_and_insert__( line, line_num );

        if( b == false )
            dummy_log_warn( MODULENAME, "cannot parse record '%s' at line %u", line.c_str(), line_num );
    }

    in_file.close();

    dummy_log_debug( MODULENAME, "loaded words = %u", id_to_word_.size() );

    return true;
}

bool GSpeak::parse_line_and_insert__( const std::string & si, uint32 line_num )
{
    std::string s( si );

    trim( s );

    if( s.empty() )
        return true;

    std::istringstream iss( s );

    WordLocale w;
    uint32 id;

    uint32  lang_int;

    iss >> id;
    iss >> lang_int;
    std::getline( iss, w.word );

    trim( w.word );

    if( w.word.empty() )
        return false;

    w.lang  = static_cast< lang_tools::lang_e >( lang_int );

    return add_new_word( w, id );
}

NAMESPACE_GSPEAK_END
