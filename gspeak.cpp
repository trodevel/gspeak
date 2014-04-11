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

// $Id: gspeak.cpp 369 2014-04-11 22:53:30Z serge $


#include "gspeak.h"           // self

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>     // boost::filesystem::exists
#include <boost/algorithm/string.hpp>   // to_lower_copy
#include <sstream>                  // std::ostringstream
#include <locale>                   // std::locale

#include <boost/functional/hash.hpp>    // boost::hash_combine

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/wrap_mutex.h"        // SCOPE_LOCK
#include "../utils/tokenizer.h"         // tokenize_to_vector
#include "wav_proc.h"                   // convert_mp3_to_wav

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "GSpeak"

NAMESPACE_GSPEAK_START

bool file_exist( const std::string& name )
{
     return boost::filesystem::exists( name );
}

class StrHelper
{
public:

static std::string to_string( const GSpeak::WordLocale & w )
{
    std::ostringstream s;

    s << w.word << " " << w.lang;

    return s.str();
}

static std::string to_string( const GSpeak::Token & t )
{
    std::ostringstream s;

    s << t.id << " " << t.lang;

    return s.str();
}
};

GSpeak::GSpeak():
    is_inited_( false )
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

    if( false == load_state__() )
    {
        dummy_log( 0, MODULENAME, "WARNING: cannot load state" );
    }

    is_inited_  = true;

    return true;
}

bool GSpeak::save_state()
{
    SCOPE_LOCK( mutex_ );

    if( !is_inited__() )
    {
        dummy_log( 0, MODULENAME, "ERROR: not inited" );
        return false;
    }

    return save_state__();
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

    say_text( ids, filename );

    return true;
}
std::string GSpeak::get_error_msg() const
{
    SCOPE_LOCK( mutex_ );

    return error_msg_;
}

void GSpeak::set_error_msg__( const std::string & s )
{
    error_msg_  = s;
}

bool GSpeak::say_text( const TokenVect & inp, const std::string & wav_file )
{
    TokenVect::const_iterator it_end    = inp.end();
    TokenVect::const_iterator it        = inp.begin();

    StrVect wav_files;

    for( ; it != it_end; ++it )
    {

        std::string file;

        const Token & t    = *it;

        bool b = generate_wav_file( t, file );

        if( b == false )
        {
            dummy_log( 0, MODULENAME, "ERROR: cannot generate wav file for token %s", StrHelper::to_string( t ).c_str() );
            return false;
        }

        wav_files.push_back( file );
    }

    join_wav_files( wav_files, wav_file );

    return true;
}

bool GSpeak::generate_wav_file( const Token & t, std::string & wav_file )
{
    std::string f_wav   = get_filename_wav( t );

    if( file_exist( f_wav ) )
    {
        // wav file already exists, nothing to do

        wav_file    = f_wav;
        return true;
    }

    std::string f_mp3   = get_filename_mp3( t );

    if( file_exist( f_mp3 ) )
    {
        // mp3 file exists, just convert it into wav

        bool b = convert_mp3_to_wav( f_mp3, f_wav );

        if( b == false )
        {
            dummy_log( 0, MODULENAME, "ERROR: cannot convert mp3 file to wav for token %s", StrHelper::to_string( t ).c_str() );
            return false;
        }

        wav_file    = f_wav;
        return true;
    }

    // nothing exists, need to generate mp3 and then convert to wav

    bool b1 = say_word( t, f_mp3 );

    if( b1 == false )
    {
        dummy_log( 0, MODULENAME, "ERROR: cannot generate mp3 file for token %s", StrHelper::to_string( t ).c_str() );
        return false;
    }

    bool b2 = convert_mp3_to_wav( f_mp3, f_wav );

    if( b2 == false )
    {
        dummy_log( 0, MODULENAME, "ERROR: cannot convert mp3 file to wav for token %s", StrHelper::to_string( t ).c_str() );
        return false;
    }

    wav_file    = f_wav;

    return true;
}

std::string GSpeak::get_filename_wav( const Token & t ) const
{
    std::ostringstream s;

    s << config_.temp_path << "/" << t.id << ".wav";

    return s.str();
}

std::string GSpeak::get_filename_mp3( const Token & t ) const
{
    std::ostringstream s;

    s << config_.data_path << "/" << t.id << ".mp3";

    return s.str();
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

std::string GSpeak::get_locale_name( ITextToSpeech::lang_e lang )
{
    static const std::string def  = "en_GB.UTF-8";
    static const std::string en  = "en_GB.UTF-8";
    static const std::string de  = "de_DE.UTF-8";
    static const std::string ru  = "ru_RU.UTF-8";
    switch( lang )
    {
    case ITextToSpeech::UNKNOWN:
        return def;
    case ITextToSpeech::EN:
        return en;
    case ITextToSpeech::DE:
        return de;
    case ITextToSpeech::RU:
        return ru;
    default:
        break;
    }

    return def;

}

void GSpeak::localize( WordLocale & w )
{
    try
    {
        std::string loc_name    = get_locale_name( w.lang );

        std::locale loc( loc_name.c_str() );

        w.word  = boost::algorithm::to_lower_copy( w.word, loc );
    }
    catch( std::runtime_error &e )
    {

    }
}

bool GSpeak::convert_words_to_tokens( const StrVect & inp, TokenVect & outp )
{
    ITextToSpeech::lang_e lang = ITextToSpeech::EN;

    for( auto s : inp )
    {
        ITextToSpeech::lang_e l = check_lang( s );

        if( l != ITextToSpeech::lang_e::UNKNOWN )
        {
            lang = l;
            continue;   // don't generate a word since it's a keyword
        }

        WordLocale w;

        w.lang  = lang;
        w.word  = s;

        localize( w );

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

uint32 GSpeak::get_word_hash( const WordLocale & w )
{
    std::size_t seed = 0;

    boost::hash_combine( seed, boost::hash_value( w.lang ) );

    boost::hash_combine( seed, boost::hash_value( w.word ) );

    return seed;
}

uint32 GSpeak::get_word_id( const WordLocale & w )
{
    auto res = word_to_id_.find( w );

    if( res == word_to_id_.end() )
    {
        uint32 id  = get_word_hash( w );

        add_new_word( w, id );

        return id;
    }

    return res->second;
}

bool GSpeak::add_new_word( const WordLocale & w, uint32 id )
{
    word_to_id_[ w ]   = id;

    Token t;

    t.id    = id;
    t.lang  = w.lang;

    id_to_word_.insert( MapTokenToString::value_type( t, w.word ) );

    return true;
}

bool GSpeak::say_word( const Token & t, const std::string & mp3_file )
{
    const std::string & w = id_to_word_[ t ];

    gtts_.say( w, mp3_file, t.lang );

    return true;
}


NAMESPACE_GSPEAK_END
