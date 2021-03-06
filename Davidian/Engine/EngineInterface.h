//
// Created by alexs on 6/23/18.
//

#ifndef DAVIDIAN_ENGINEINTERFACE_H
#define DAVIDIAN_ENGINEINTERFACE_H

#include <optional>
#include <string>
#include <Engine.pb.h>

namespace Davidian{
namespace engine{
class System;
} // namespace engine

namespace orbital{
class StateVector;
} // namespace orbital

} // namespace Davidian

namespace engine{

using System_proto = Davidian::engine::System;
using OrbitState_proto = Davidian::engine::OrbitState;

class EngineInterface{
public:
  virtual ~EngineInterface() = default;

  virtual void loadSystem(const std::string& filename) = 0;
  virtual void useSystem(const System_proto& system) = 0;

  virtual bool hasValidSystem() const = 0;

  virtual System_proto getCurrentSystem() const = 0;

  /**
   * Without evolving the state (transfers, eg) get the state vector for a named body at a given time
   * @param bodyName The name of the body
   * @param seconds time in seconds
   * @return optional state vector. If the body exists and the system is valid, it will be valued with the state vector
   * of the body at the given time. Else, it will not be valued.
   */
  virtual std::optional<OrbitState_proto> bodyStateAtTime(const std::string& bodyName, double seconds) const = 0;
};
} // namespace engine

#endif //DAVIDIAN_ENGINEINTERFACE_H
