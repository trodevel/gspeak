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

// $Id: gspeak.h 341 2014-04-02 17:05:22Z serge $

#ifndef GSPEAK_H
#define GSPEAK_H

#include <string>                   // std::string
#include <boost/thread.hpp>         // boost::mutex

#include <map>                      // std::map
#include <vector>                   // std::vector


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
private:

    struct WordLocale
    {
        ITextToSpeech::lang_e   lang;
        std::string             word;

        bool operator<( const WordLocale & rh ) const
        {
            if( lang < rh.lang && word < rh.word )
                return true;

            return false;
        }
    };


    typedef std::map< WordLocale, uint32 >     MapStrToInt;


    struct Token
    {
        uint32                  id;
        ITextToSpeech::lang_e   lang;
    };

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

    bool convert_words_to_tokens( const StrVect & inp, TokenVect & outp );

    uint32 get_word_id( const WordLocale & w );

    bool add_new_word( const WordLocale & w, uint32 id );

    bool say_word( const std::string & word, const std::string & wav_file );

    static ITextToSpeech::lang_e   check_lang( const std::string & s );

public:


private:
    mutable boost::mutex        mutex_;

    bool                        is_inited_;

    uint32                      last_id_;

    Config                      config_;

    MapStrToInt                 map_;
};

NAMESPACE_GSPEAK_END

#endif  // GSPEAK_H
