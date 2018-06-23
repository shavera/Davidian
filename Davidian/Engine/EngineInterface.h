//
// Created by alexs on 6/23/18.
//

#ifndef DAVIDIAN_ENGINEINTERFACE_H
#define DAVIDIAN_ENGINEINTERFACE_H

#include <optional>
#include <string>

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
using StateVector_proto = Davidian::orbital::StateVector;

class EngineInterface{
public:
  virtual ~EngineInterface(){};

  virtual void loadSystem(const std::string& filename) = 0;
  virtual void useSystem(const System_proto& system) = 0;

  virtual bool hasValidSystem() const = 0;

  virtual System_proto getCurrentSystem() = 0;

  /**
   * Evolve the system forward t seconds, including any possible child transfers;
   * New system will use this time as the new '0'/epoch for Mean Anomaly.
   * @param t time in seconds to advance forward
   * @return A system proto of the new system configuration
   */
  virtual System_proto advanceSystemToTime(const double t) = 0;

  /**
   * Without evolving the state (transfers, eg) get the state vector for a named body at a given time
   * @param bodyName The name of the body
   * @param t time in seconds
   * @return optional state vector. If the body exists and the system is valid, it will be valued with the state vector
   * of the body at the given time. Else, it will not be valued.
   */
  virtual std::optional<StateVector_proto> bodyStateAtTime(const std::string& bodyName, const double t) const = 0;
};
} // namespace engine

#endif //DAVIDIAN_ENGINEINTERFACE_H
