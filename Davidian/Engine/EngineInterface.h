//
// Created by alexs on 6/23/18.
//

#ifndef DAVIDIAN_ENGINEINTERFACE_H
#define DAVIDIAN_ENGINEINTERFACE_H

#include <string>

namespace Davidian{
namespace engine{
class System;
} // namespace engine

namespace orbitial{
class CartesianVector;
} // namespace orbital

} // namespace Davidian

namespace engine{

using System_proto = Davidian::engine::System;
using CartesianVector_proto = Davidian::orbitial::CartesianVector;

class EngineInterface{
public:
  virtual ~EngineInterface(){};
  virtual void loadSystem(const std::string& filename) = 0;

  virtual System_proto getCurrentSystem() = 0;
};
} // namespace engine

#endif //DAVIDIAN_ENGINEINTERFACE_H
