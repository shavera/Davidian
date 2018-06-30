//
// Created by alexs on 6/24/18.
//

#ifndef DAVIDIAN_CELESTIALSYSTEM_H
#define DAVIDIAN_CELESTIALSYSTEM_H

#include <memory>
#include <vector>

namespace Davidian{
namespace engine{
class System;
} // namespace engine
} // namesapace Davidian

namespace orbital{
class CelestialBody;
class Body;
class Orbit;
} // namespace orbital

namespace engine {

class CelestialSystem {
  using System_proto = Davidian::engine::System;
public:
  explicit CelestialSystem(const System_proto& systemProto);
  ~CelestialSystem();

  orbital::Body* body(const std::string& bodyName);

private:
  std::vector<std::unique_ptr<orbital::Body>> m_bodies;
};

} // namespace engine

#endif //DAVIDIAN_CELESTIALSYSTEM_H
