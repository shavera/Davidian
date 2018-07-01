//
// Created by alexs on 6/23/18.
//

#ifndef DAVIDIAN_UNITTESTHELPERS_H
#define DAVIDIAN_UNITTESTHELPERS_H

#include "Engine.pb.h"

#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>
#include <GlobalDefinitions.h>

namespace engine {

class TestProtoInterface{
public:
  TestProtoInterface(){
    protoDifferencer.ReportDifferencesToString(&differenceString);
  }
  virtual ~TestProtoInterface() = default;

  void compareProtos(const google::protobuf::Message& m1, const google::protobuf::Message& m2);

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
};

/// @brief creates a set of more-or-less arbitrary orbital element parameters
orbital::OrbitalElements retrieveTestOrbitalElements();

/// @brief creates a system using the above parameters plus some more arbitrary ones
Davidian::engine::System createTestSystem();

/// @brief creates a system with a child body in a unit circle orbit
Davidian::engine::System unitCircleSystem();

/// @brief creates a system with no root body for testing error cases
Davidian::engine::System createRootlessSystem();

/// @brief create a system with a complicated structure and disordered bodies
/// Creates Kerbol, Kerbin and moons, free ship in LKO, Duna/Ike, but orders them (used random order generator):
/// Ship - Mun - Kerbin - Duna - Ike - Kerbol - Minmus
Davidian::engine::System createComplicatedSystem();

} // namespace engine

#endif //DAVIDIAN_UNITTESTHELPERS_H
