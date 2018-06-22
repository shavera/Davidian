//
// Created by alexs on 6/20/18.
//

#ifndef DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H
#define DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H

#include "Engine.pb.h"

namespace engine {

class CelestialSystemFileManager {
public:
  Davidian::engine::System loadSystem(const std::string& filename) const;

  void saveSystem(const Davidian::engine::System& system, const std::string& filename) const;
};

} // namespace engine

#endif //DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H
