//
// Created by alexs on 6/20/18.
//

#ifndef DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H
#define DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H

#include "Engine.pb.h"

namespace engine {

class CelestialSystemFileManager {
public:
  virtual ~CelestialSystemFileManager();

  virtual Davidian::engine::System loadSystem(const std::string& filename) const;

  virtual void saveSystem(const Davidian::engine::System& system, const std::string& filename) const;
};

} // namespace engine

#endif //DAVIDIAN_CELESTIALSYSTEMFILEMANAGER_H
