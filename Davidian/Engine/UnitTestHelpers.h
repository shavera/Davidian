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
  virtual ~TestProtoInterface(){}

  void compareProtos(const google::protobuf::Message& m1, const google::protobuf::Message& m2);

  google::protobuf::util::MessageDifferencer protoDifferencer;
  std::string differenceString;
};

Davidian::engine::System createTestSystem();

} // namespace engine

#endif //DAVIDIAN_UNITTESTHELPERS_H
