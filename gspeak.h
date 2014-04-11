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

// $Id: gspeak.h 361 2014-04-10 17:13:22Z serge $

#ifndef GSPEAK_H
#define GSPEAK_H

#include <string>                   // std::string
#include <boost/thread.hpp>         // boost::mutex

#include <map>                      // std::map
#include <vector>                   // std::vector

#include "gtts.h"                   // Gtts


#include "i_text_to_speech.h"       // ITextToSpeech

#include "namespace_gspeak.h"  // NAMESPACE_GSPEAK_START


NAMESPACE_GSPEAK_START

struct Config
{
    std::string     word_base_path;
    std::string     data_path;
    std::string     temp_path;
};

class GSpeak: virtual public ITextToSpeech
{
    friend class StrHelper;
//private:
public:

    struct WordLocale
    {

        ITextToSpeech::lang_e   lang;
        std::string             word;

        bool operator<( const WordLocale & rh ) const
        {
            if( lang < rh.lang )
                return true;

            if( lang == rh.lang && word < rh.word )
                return true;

            return false;
        }
    };

    struct Token
    {
        //friend class boost::serialization::access;

        uint32                  id;
        ITextToSpeech::lang_e   lang;

        bool operator<( const Token & rh ) const
        {
            if( lang < rh.lang )
                return true;

            if( lang == rh.lang && id < rh.id )
                return true;

            return false;
        }
    };

    typedef std::map< WordLocale, uint32 >      MapWordLocaleToInt;
    typedef std::map< Token, std::string >      MapTokenToString;

    typedef std::vector< std::string >          StrVect;
    typedef std::vector< Token >                TokenVect;

public:
    GSpeak();

    ~GSpeak();

    bool init( const Config & config );
    bool save_state();

    // interface of ITextToSpeech
    bool say( const std::string & text, const std::string & filename, lang_e lang = lang_e::EN );
    std::string get_error_msg() const;


private:

    bool is_inited__() const;

    bool save_state__();

    bool load_state__();

    void set_error_msg__( const std::string & s );

    bool convert_words_to_tokens( const StrVect & inp, TokenVect & outp );

    bool say_text( const TokenVect & inp, const std::string & wav_file );

    uint32 get_word_id( const WordLocale & w );

    bool add_new_word( const WordLocale & w, uint32 id );

    bool generate_wav_file( const Token & t, std::string & wav_file );

    std::string get_filename_wav( const Token & t ) const;
    std::string get_filename_mp3( const Token & t ) const;

    bool say_word( const Token & t, const std::string & mp3_file );

    static ITextToSpeech::lang_e   check_lang( const std::string & s );

public:


private:
    mutable boost::mutex        mutex_;

    bool                        is_inited_;

    uint32                      last_id_;

    Config                      config_;

    MapWordLocaleToInt          word_to_id_;
    MapTokenToString            id_to_word_;

    Gtts                        gtts_;

    std::string                 error_msg_;
};

NAMESPACE_GSPEAK_END

#endif  // GSPEAK_H
