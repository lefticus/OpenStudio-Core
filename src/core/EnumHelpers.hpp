/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_CORE_ENUMHELPERS_HPP
#define UTILITIES_CORE_ENUMHELPERS_HPP

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>

namespace openstudio {

template <class _enum_name>
bool isMember(const _enum_name& e, const StringVector& strValues) {
  BOOST_FOREACH(const std::string& str, strValues) {
    try {
      _enum_name setMember(str);
      if (e == setMember) { return true; }
    }
    catch (...) {}
  }
  return false;
}

} // openstudio

#endif // UTILITIES_CORE_ENUMHELPERS_HPP
