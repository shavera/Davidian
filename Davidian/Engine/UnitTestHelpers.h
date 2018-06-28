//
// Created by alexs on 6/23/18.
//

#ifndef DAVIDIAN_UNITTESTHELPERS_H
#define DAVIDIAN_UNITTESTHELPERS_H

#include "Engine.pb.h"

#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>

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

/// @brief creates a system with more-or-less arbitrary values for testing
Davidian::engine::System createTestSystem();

/// @brief creates a system with a child body in a unit circle orbit
Davidian::engine::System unitCircleSystem();

/// @brief creates a system with no root body for testing error cases
Davidian::engine::System createRootlessSystem();

} // namespace engine

#endif //DAVIDIAN_UNITTESTHELPERS_H
