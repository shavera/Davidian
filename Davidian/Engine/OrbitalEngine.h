//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITALENGINE_H
#define DAVIDIAN_ORBITALENGINE_H

#include <memory>

namespace engine {

class CelestialSystemFileManager;

class OrbitalEngine{
public:
  OrbitalEngine();

protected:
  explicit OrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager);

private:
  std::unique_ptr<CelestialSystemFileManager> m_fileManager;
};

} // namespace engine

#endif //DAVIDIAN_ORBITALENGINE_H
