//
// Created by alexs on 5/19/17.
//

#ifndef DAVIDIAN_ORBITIMPL_H
#define DAVIDIAN_ORBITIMPL_H

#include "GlobalDefinitions.h"

namespace orbital{
namespace impl{

class OrbitImpl {
public:
    double energy(const StateVector& stateVector, double standardGravitationalParameter);
};

} // namespace impl
} // namespace orbital

#endif //DAVIDIAN_ORBITIMPL_H
