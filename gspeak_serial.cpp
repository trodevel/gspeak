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

// $Revision: 1404 $ $Date:: 2015-01-16 #$ $Author: serge $


#include "gspeak.h"           // self

#include <boost/bind.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

#include <sstream>                  // std::ostringstream
#include <fstream>                  // std::ofstream

#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/wrap_mutex.h"        // SCOPE_LOCK

#include "namespace_gspeak.h"       // NAMESPACE_GSPEAK_START

#define MODULENAME      "GSpeak"

namespace boost {
namespace serialization {

template<class Archive>
void serialize( Archive & ar, gspeak::GSpeak::Token & g, const unsigned int version )
{
    ar & g.id;
    ar & g.lang;
}

} // namespace serialization
} // namespace boost

NAMESPACE_GSPEAK_START

bool GSpeak::save_state__()
{
    std::ofstream ofs( config_.word_base_path );

    boost::archive::text_oarchive oa( ofs );

    dummy_log_debug( MODULENAME, "size = %u", id_to_word_.size() );

    oa << BOOST_SERIALIZATION_NVP( id_to_word_ );

    return true;
}

bool GSpeak::load_state__()
{
    try
    {
        std::ifstream fs( config_.word_base_path );

        boost::archive::text_iarchive oa( fs );

        oa >> BOOST_SERIALIZATION_NVP( id_to_word_ );

        dummy_log_debug( MODULENAME, "load size = %u", id_to_word_.size() );

        return true;
    }
    catch( std::exception & e )
    {
        set_error_msg__( std::string( "got exception " ) + e.what() );
    }

    return false;
}

NAMESPACE_GSPEAK_END
