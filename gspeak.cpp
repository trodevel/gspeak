/*

Cache over Gtts.

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

// $Id: gspeak.cpp 341 2014-04-02 17:05:22Z serge $


#include "gspeak.h"           // self

#include <boost/bind.hpp>
#include <sstream>                  // std::ostringstream

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/wrap_mutex.h"        // SCOPE_LOCK
#include "../utils/tokenizer.h"         // tokenize_to_vector

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "GSpeak"

NAMESPACE_GSPEAK_START

GSpeak::GSpeak():
    is_inited_( false ), last_id_( 0 )
{
}

GSpeak::~GSpeak()
{
}

bool GSpeak::init( const Config & config )
{
    SCOPE_LOCK( mutex_ );

    if( is_inited__() == true )
    {
        dummy_log( 0, MODULENAME, "ERROR: already inited" );
        return false;
    }

    if( config.word_base_path.empty() )
    {
        dummy_log( 0, MODULENAME, "ERROR: word base path is empty" );
        return false;
    }

    if( config.data_path.empty() )
    {
        dummy_log( 0, MODULENAME, "ERROR: data path is empty" );
        return false;
    }

    if( config.temp_path.empty() )
    {
        dummy_log( 0, MODULENAME, "ERROR: temp path is empty" );
        return false;
    }

    config_ = config;

    is_inited_  = true;

    return true;
}

bool GSpeak::is_inited__() const
{
    return is_inited_;
}

bool GSpeak::say( const std::string & text, const std::string & filename, lang_e lang )
{
    SCOPE_LOCK( mutex_ );

    if( !is_inited__() )
    {
        dummy_log( 0, MODULENAME, "ERROR: not inited" );
        return false;
    }

    std::vector< std::string > words;

    tokenize_to_vector( words, text, " " );

    if( words.empty() )
        return true;

    TokenVect ids;

    convert_words_to_tokens( words, ids );

    return true;
}
std::string GSpeak::get_error_msg() const
{
    SCOPE_LOCK( mutex_ );

    return "";
}

ITextToSpeech::lang_e   GSpeak::check_lang( const std::string & s )
{
    if( s == "<en>" )
        return ITextToSpeech::EN;
    if( s == "<de>" )
        return ITextToSpeech::DE;
    if( s == "<ru>" )
        return ITextToSpeech::RU;

    return ITextToSpeech::UNKNOWN;
}

bool GSpeak::convert_words_to_tokens( const StrVect & inp, TokenVect & outp )
{
    ITextToSpeech::lang_e lang = ITextToSpeech::EN;

    for( int i = 0; i < ( int ) inp.size(); ++i )
    {
        const std::string & s = inp[i];

        ITextToSpeech::lang_e l = check_lang( s );

        if( l != ITextToSpeech::lang_e::UNKNOWN )
        {
            lang = l;
            continue;   // don't generate a word since it's a keyword
        }

        WordLocale w;

        w.lang  = lang;
        w.word  = s;

        uint32                  id = get_word_id( w );

        if( id == 0 )
        {
            dummy_log( 0, MODULENAME, "ERROR: cannot get id for word '%s'", s.c_str() );
            return false;
        }

        Token t;

        t.id    = id;
        t.lang  = lang;

        outp.push_back( t );
    }

    return true;
}

uint32 GSpeak::get_word_id( const WordLocale & w )
{
    if( map_.count( w ) == 0 )
    {
        last_id_++;

        add_new_word( w, last_id_ );

        return last_id_;
    }

    return map_[ w ];
}

bool GSpeak::add_new_word( const WordLocale & w, uint32 id )
{
    return true;
}

bool GSpeak::say_word( const std::string & word, const std::string & wav_file )
{
    return true;
}


NAMESPACE_GSPEAK_END
