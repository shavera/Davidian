//
// Created by alexs on 6/23/18.
//

#include "UnitTestHelpers.h"

#include <gtest/gtest.h>
#include <GlobalDefinitions.h>

using System_proto = Davidian::engine::System;

namespace engine{

namespace{
constexpr double degToRad(double deg){return (deg/180)*M_PI;}
} // anonymous namespace

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

Davidian::engine::System createComplicatedSystem() {
  System_proto system;

  auto* ship = system.add_body();
  ship->set_mass(1.0e8);
  ship->set_name("Ship");
  ship->mutable_free_body()->set_parent_body_name("Kerbin");
  auto* shipOrbit = ship->mutable_free_body()->mutable_orbit();
  shipOrbit->set_semimajor_axis(1.6e6);

  auto* mun = system.add_body();
  mun->set_mass(9.759066e20);
  mun->set_name("Mun");
  mun->mutable_celestial_body()->set_parent_body_name("Kerbin");
  auto* munOrbit = mun->mutable_celestial_body()->mutable_orbit();
  munOrbit->set_semimajor_axis(12'000'000);
  // all orbital values default to 0
  munOrbit->set_mean_anomaly_0(1.7);

  auto* kerbin = system.add_body();
  kerbin->set_mass(5.2915158e22);
  kerbin->set_name("Kerbin");
  kerbin->mutable_celestial_body()->set_parent_body_name("Kerbol");
  auto* kerbinOrbit = kerbin->mutable_celestial_body()->mutable_orbit();
  kerbinOrbit->set_semimajor_axis(13'599'840'256);
  // all orbital values default to 0
  kerbinOrbit->set_mean_anomaly_0(3.14);

  auto* duna = system.add_body();
  duna->set_mass(4.5154270e21);
  duna->set_name("Duna");
  duna->mutable_celestial_body()->set_parent_body_name("Kerbol");
  auto* dunaOrbit = duna->mutable_celestial_body()->mutable_orbit();
  dunaOrbit->set_semimajor_axis(20'726'155'264);
  dunaOrbit->set_eccentricity(0.051);
  dunaOrbit->set_inclination(degToRad(0.06));
  dunaOrbit->set_longitude_asc_node(degToRad(135.5));
  dunaOrbit->set_arg_periapsis(0.0);
  dunaOrbit->set_mean_anomaly_0(3.14);

  auto* ike = system.add_body();
  ike->set_mass(2.7821615e20);
  ike->set_name("Ike");
  ike->mutable_celestial_body()->set_parent_body_name("Duna");
  auto* ikeOrbit = ike->mutable_celestial_body()->mutable_orbit();
  ikeOrbit->set_semimajor_axis(3'200'000);
  ikeOrbit->set_eccentricity(0.03);
  ikeOrbit->set_inclination(degToRad(0.2));
  ikeOrbit->set_mean_anomaly_0(1.7);

  auto* kerbol = system.add_body();
  kerbol->mutable_root_body();
  kerbol->set_mass(1.7565459e28);
  kerbol->set_name("Kerbol");

  auto* minmus = system.add_body();
  minmus->set_mass(2.6457580e19);
  minmus->set_name("Minmus");
  minmus->mutable_celestial_body()->set_parent_body_name("Kerbin");
  auto* minmusOrbit = minmus->mutable_celestial_body()->mutable_orbit();
  minmusOrbit->set_semimajor_axis(47'000'000);
  minmusOrbit->set_eccentricity(0);
  minmusOrbit->set_inclination(degToRad(6));
  minmusOrbit->set_longitude_asc_node(degToRad(78));
  minmusOrbit->set_arg_periapsis(degToRad(38));
  minmusOrbit->set_mean_anomaly_0(0.9);

  return system;
}

void TestProtoInterface::compareProtos(const google::protobuf::Message& m1, const google::protobuf::Message& m2){
  EXPECT_TRUE(protoDifferencer.Compare(m1, m2)) << differenceString;
}

} // namespace engine