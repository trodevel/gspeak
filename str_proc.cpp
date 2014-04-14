/*

String manipulation functions.

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

// $Id: str_proc.cpp 381 2014-04-14 23:31:28Z serge $


#include "str_proc.h"               // self

#include <set>                      // std::set
#include <algorithm>                // std::unique

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/tokenizer.h"         // tokenize_to_vector
#include "../utils/trim.h"          // trim

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "StrProc"

NAMESPACE_GSPEAK_START

void split( std::vector<std::string> & res, const std::string & str )
{
    std::string temp;

    static const std::set< char > delim = { '.', ',', ';', '!', '?' };

    static auto not_found = delim.end();

    enum state_e
    {
        NORM,
        KEYW
    };

    const char BR1 = '<';
    const char BR2 = '>';

    state_e state = NORM;

    for( auto c : str )
    {
        switch( state )
        {
        case NORM:
            if( c == BR1 )
            {
                if( !temp.empty() )
                {
                    res.push_back( temp );
                    temp.clear();
                }
                temp.append( 1, c );

                state = KEYW;
            }
            else if( delim.find( c ) != not_found )
            {
                if( !temp.empty() )
                {
                    res.push_back( temp );
                    temp.clear();
                }
            }
            else
            {
                temp.append( 1, c );
            }
            break;

        case KEYW:
            if( c == BR2 )
            {
                if( !temp.empty() )
                {
                    temp.append( 1, c );

                    res.push_back( temp );
                    temp.clear();
                }

                state = NORM;
            }
            else
            {
                temp.append( 1, c );
            }
            break;

        default:
            break;
        }
    }

    if( !temp.empty() )
    {
        res.push_back( temp );
    }
}

void split_into_sentences( std::vector<std::string> & res, const std::string & str )
{
    split( res, str );

    for( auto & s : res )
    {
        trim( s );
        remove_extra_spaces( s );
    }
}

bool BothAreSpaces( char lhs, char rhs )
{
    return ( lhs == rhs ) && ( lhs == ' ' );
}

void remove_extra_spaces( std::string & s )
{
    // stackoverflow.com/questions/8362094/replace-multiple-spaces-with-one-space-in-a-string

    std::string::iterator new_end = std::unique( s.begin(), s.end(), BothAreSpaces );
    s.erase( new_end, s.end() );
}

NAMESPACE_GSPEAK_END
