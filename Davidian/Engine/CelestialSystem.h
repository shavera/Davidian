//
// Created by alexs on 6/24/18.
//

#ifndef DAVIDIAN_CELESTIALSYSTEM_H
#define DAVIDIAN_CELESTIALSYSTEM_H

#include <memory>
#include <unordered_map>

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

class OrbitalHistory;
using OrbitalHistoryMap = std::unordered_map<std::string, std::unique_ptr<OrbitalHistory>>;

class CelestialSystem {
public:
  /**
   * Construct CelestialSystem from System proto
   *
   * @warning Will require precisely one root body. With less than 1 root body, will generate none. With more than 1
   * root body, behaviour may be undefined. Will try to construct based on the first root body found. But no validation
   * is currently being performed on checking for multiple root bodies.
   *
   * @param systemProto Protobuf describing the system
   */
  explicit CelestialSystem(const Davidian::engine::System& systemProto);
  ~CelestialSystem();

  bool isValidSystem() const;

  const orbital::Body* body(const std::string& bodyName) const;
  orbital::Body* body(const std::string& bodyName);

  Davidian::engine::System constructCurrentSystem() const;

  /**
   * Calculate orbital histories for all bodies in the Celestial System.
   *
   * @note if there is a costly function call in this whole program, this will probably be it.
   * Consider how parallelization might be an effective tool here.
   *
   * @returns a map of body names to orbital histories to refer to.
   */
  OrbitalHistoryMap calculateHistories() const;

private:
  std::unordered_map<std::string, std::unique_ptr<orbital::Body>> m_bodies;
};

} // namespace engine

#endif //DAVIDIAN_CELESTIALSYSTEM_H
