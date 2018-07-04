//
// Created by alexs on 6/20/18.
//

#include "CelestialSystemFileManager.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>

using Davidian::engine::System;

namespace engine {

System CelestialSystemFileManager::loadSystem(const std::string &filename) const {
  System system;

  auto* file = std::fopen(filename.c_str(), "r");
  if(nullptr == file) {return system;}

  google::protobuf::io::FileInputStream inputStream{fileno(file)};
  system.ParseFromZeroCopyStream(&inputStream);
  inputStream.Close();
  return system;
}

void CelestialSystemFileManager::saveSystem(const System &system, const std::string &filename) const {
  auto* file = std::fopen(filename.c_str(), "w");
  if(nullptr == file) {return;}

  google::protobuf::io::FileOutputStream outputStream{fileno(file)};
  system.SerializeToZeroCopyStream(&outputStream);
  outputStream.Close();
}

CelestialSystemFileManager::~CelestialSystemFileManager() = default;

} // namespace engine
