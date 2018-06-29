//
// Created by alexs on 6/23/18.
//

#include "UnitTestHelpers.h"

#include <gtest/gtest.h>
#include <GlobalDefinitions.h>

using System_proto = Davidian::engine::System;

namespace engine{

orbital::OrbitalElements retrieveTestOrbitalElements() {
  return orbital::OrbitalElements{12.34, 0.6, 1.75, 0.12, 0.45, 2.33};
}

System_proto createTestSystem() {
  System_proto system;

  auto* rootBody = system.add_body();
  rootBody->mutable_root_body();
  rootBody->set_mass(12340);
  rootBody->set_name("Root Body");

  auto* childBody = system.add_body();
  childBody->set_mass(567);
  childBody->set_name("Child");
  childBody->mutable_celestial_body()->set_parent_body_name("Root Body");
  auto* childOrbit = childBody->mutable_celestial_body()->mutable_orbit();
  auto elements = retrieveTestOrbitalElements();
  childOrbit->set_semimajor_axis(elements.semiMajorAxis);
  childOrbit->set_eccentricity(elements.eccentricity);
  childOrbit->set_longitude_asc_node(elements.longitudeOfAscendingNode);
  childOrbit->set_inclination(elements.inclination);
  childOrbit->set_arg_periapsis(elements.argumentOfPeriapsis);
  childOrbit->set_mean_anomaly_0(elements.meanAnomalyAtEpoch);

  return system;
}

Davidian::engine::System unitCircleSystem() {
  System_proto system;

  auto* rootBody = system.add_body();
  rootBody->mutable_root_body();
  rootBody->set_mass(1/orbital::G);
  rootBody->set_name("Root Body");

  auto* childBody = system.add_body();
  childBody->set_mass(0);
  childBody->set_name("Child");
  childBody->mutable_celestial_body()->set_parent_body_name("Root Body");
  auto* childOrbit = childBody->mutable_celestial_body()->mutable_orbit();
  childOrbit->set_semimajor_axis(1);
  childOrbit->set_eccentricity(1);
  // all other values default to 0

  return system;
}

Davidian::engine::System createRootlessSystem(){
  System_proto system;

  auto* childBody = system.add_body();
  childBody->set_mass(0);
  childBody->set_name("Child");
  childBody->mutable_celestial_body()->set_parent_body_name("Root Body");
  auto* childOrbit = childBody->mutable_celestial_body()->mutable_orbit();
  childOrbit->set_semimajor_axis(1);
  childOrbit->set_eccentricity(1);
  // all other values default to 0

  return system;

}

void TestProtoInterface::compareProtos(const google::protobuf::Message& m1, const google::protobuf::Message& m2){
  EXPECT_TRUE(protoDifferencer.Compare(m1, m2)) << differenceString;
}

} // namespace engine