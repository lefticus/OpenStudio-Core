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

#include <gtest/gtest.h>
#include <utilities/bcl/test/BCLFixture.hpp>
#include <resources.hxx>

#include <utilities/bcl/BCLComponent.hpp>

using namespace openstudio;

TEST_F(BCLFixture, BCLComponent)
{
  std::string dir = toString(resourcesPath()) + "/utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/";
  BCLComponent component(dir);

  // "_" has been replaced by " "
  EXPECT_EQ("philadelphia pa [724086 TMY2-13739]", component.name());
}
