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

// $Revision: 1573 $ $Date:: 2015-03-12 #$ $Author: serge $

#ifndef STR_PROC_H
#define STR_PROC_H

#include <string>                   // std::string

#include <vector>                   // std::vector

#include "namespace_lib.h"  // NAMESPACE_GSPEAK_START

NAMESPACE_GSPEAK_START


void split_into_sentences( std::vector<std::string> & res, const std::string & str );

void remove_special_symbols( std::string & s );

void remove_extra_spaces( std::string & s );

NAMESPACE_GSPEAK_END

#endif  // STR_PROC_H
