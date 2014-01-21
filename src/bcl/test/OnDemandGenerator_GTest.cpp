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

#include <utilities/bcl/BCLComponent.hpp>
#include <utilities/bcl/LocalBCL.hpp>
#include <utilities/bcl/RemoteBCL.hpp>
#include <utilities/bcl/OnDemandGenerator.hpp>

using namespace openstudio;


TEST_F(BCLFixture, OnDemandGeneratorTest)
{
  RemoteBCL remoteBCL;
  remoteBCL.setProdAuthKey(prodAuthKey);
  remoteBCL.setDevAuthKey(devAuthKey);
  //remoteBCL.useRemoteDevelopmentUrl();
 
  // space type generator
  bool test = remoteBCL.downloadOnDemandGenerator("bb8aa6a0-6a25-012f-9521-00ff10704b07");
  ASSERT_TRUE(test);
  boost::optional<OnDemandGenerator> generator = remoteBCL.waitForOnDemandGenerator();
  ASSERT_TRUE(generator);

  // no argument values set yet
  EXPECT_FALSE(generator->checkArgumentValues());
  EXPECT_FALSE(LocalBCL::instance().getOnDemandComponent(*generator));

  // check args
  std::vector<OnDemandGeneratorArgument> arguments = generator->arguments();
  ASSERT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  // cannot set climate zone before picking vintage
  EXPECT_FALSE(generator->setArgumentValue("Climate Zone", "ClimateZone 1-3"));

  // at first, vintage is only argument
  arguments = generator->arguments();
  ASSERT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  std::vector<std::string> activeArgumentNames = generator->activeArgumentNames();
  ASSERT_EQ(1u, activeArgumentNames.size());
  EXPECT_EQ("NREL_reference_building_vintage", activeArgumentNames[0]);
  boost::optional<OnDemandGeneratorArgument> argument = generator->getActiveArgument(activeArgumentNames[0]);
  ASSERT_TRUE(argument);
  EXPECT_EQ("NREL_reference_building_vintage", argument->name());
  EXPECT_EQ("Standard", argument->displayName());
  std::vector<OnDemandGeneratorValueRestriction> valueRestrictions = argument->valueRestrictions();
  ASSERT_EQ(4u, valueRestrictions.size());
  EXPECT_EQ("ASHRAE_189.1-2009", valueRestrictions[0].name());
  EXPECT_FALSE(generator->setArgumentValue(argument->name(), "Junk"));
  EXPECT_TRUE(generator->setArgumentValue(argument->name(), valueRestrictions[0].name()));

  // not ready yet
  EXPECT_FALSE(generator->checkArgumentValues());

  // now climate zone is an active argument
  arguments = generator->arguments();
  EXPECT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  activeArgumentNames = generator->activeArgumentNames();
  ASSERT_EQ(2u, activeArgumentNames.size());
  EXPECT_EQ("NREL_reference_building_vintage", activeArgumentNames[0]);
  EXPECT_EQ("Climate_zone", activeArgumentNames[1]);
  argument = generator->getActiveArgument(activeArgumentNames[1]);
  ASSERT_TRUE(argument);
  EXPECT_EQ("Climate_zone", argument->name());
  EXPECT_EQ("Climate Zone", argument->displayName());
  valueRestrictions = argument->valueRestrictions();
  ASSERT_EQ(2u, valueRestrictions.size());
  EXPECT_EQ("ClimateZone 1-3", valueRestrictions[0].name());
  EXPECT_TRUE(generator->setArgumentValue(argument->name(), valueRestrictions[0].name()));
  
  // not ready yet
  EXPECT_FALSE(generator->checkArgumentValues());

  // now primary space type is an active argument
  arguments = generator->arguments();
  EXPECT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  activeArgumentNames = generator->activeArgumentNames();
  ASSERT_EQ(3u, activeArgumentNames.size());
  EXPECT_EQ("NREL_reference_building_vintage", activeArgumentNames[0]);
  EXPECT_EQ("Climate_zone", activeArgumentNames[1]);
  EXPECT_EQ("NREL_reference_building_primary_space_type", activeArgumentNames[2]);
  argument = generator->getActiveArgument(activeArgumentNames[2]);
  ASSERT_TRUE(argument);
  EXPECT_EQ("NREL_reference_building_primary_space_type", argument->name());
  EXPECT_EQ("Primary Space Type", argument->displayName());
  valueRestrictions = argument->valueRestrictions();
  ASSERT_EQ(16u, valueRestrictions.size());
  EXPECT_EQ("FullServiceRestaurant", valueRestrictions[0].name());
  EXPECT_TRUE(generator->setArgumentValue(argument->name(), valueRestrictions[0].name()));

  // not ready yet
  EXPECT_FALSE(generator->checkArgumentValues());

  // now secondary space type is an active argument
  arguments = generator->arguments();
  EXPECT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  activeArgumentNames = generator->activeArgumentNames();
  ASSERT_EQ(4u, activeArgumentNames.size());
  EXPECT_EQ("NREL_reference_building_vintage", activeArgumentNames[0]);
  EXPECT_EQ("Climate_zone", activeArgumentNames[1]);
  EXPECT_EQ("NREL_reference_building_primary_space_type", activeArgumentNames[2]);
  EXPECT_EQ("NREL_reference_building_secondary_space_type", activeArgumentNames[3]);
  argument = generator->getActiveArgument(activeArgumentNames[3]);
  ASSERT_TRUE(argument);
  EXPECT_EQ("NREL_reference_building_secondary_space_type", argument->name());
  EXPECT_EQ("Secondary Space Type", argument->displayName());
  valueRestrictions = argument->valueRestrictions();
  ASSERT_EQ(2u, valueRestrictions.size());
  EXPECT_EQ("Dining", valueRestrictions[0].name());
  EXPECT_TRUE(generator->setArgumentValue(argument->name(), valueRestrictions[0].name()));

  // now it is ready
  EXPECT_TRUE(generator->checkArgumentValues());

  arguments = generator->arguments();
  EXPECT_EQ(1u, arguments.size());
  EXPECT_EQ("Standard", arguments[0].displayName());

  activeArgumentNames = generator->activeArgumentNames();
  ASSERT_EQ(4u, activeArgumentNames.size());
  EXPECT_EQ("NREL_reference_building_vintage", activeArgumentNames[0]);
  EXPECT_EQ("Climate_zone", activeArgumentNames[1]);
  EXPECT_EQ("NREL_reference_building_primary_space_type", activeArgumentNames[2]);
  EXPECT_EQ("NREL_reference_building_secondary_space_type", activeArgumentNames[3]);

  // remove this component if we already have it
  boost::optional<BCLComponent> component = LocalBCL::instance().getOnDemandComponent(*generator);
  if (component){
    std::string directory = component->directory();
    EXPECT_TRUE(boost::filesystem::exists(toPath(directory)));
    bool test = LocalBCL::instance().removeComponent(*component);
    EXPECT_TRUE(test);
    EXPECT_FALSE(boost::filesystem::exists(toPath(directory)));
  }
  EXPECT_FALSE(LocalBCL::instance().getOnDemandComponent(*generator));

  // call the generator
  test = remoteBCL.callOnDemandGenerator(*generator);
  ASSERT_TRUE(test);

  component = remoteBCL.waitForComponentDownload(300000);  // 5 minutes
  ASSERT_TRUE(component);

  // check we can find this locally
  EXPECT_TRUE(generator->checkArgumentValues());
  EXPECT_TRUE(LocalBCL::instance().getComponent(component->uid(), component->versionId()));
  EXPECT_TRUE(LocalBCL::instance().getOnDemandComponent(*generator));
}
