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
    OrbitImpl(const StateVector& stateVector, const double barymass, const double leptomass);
//    virtual double energy(const StateVector& stateVector, double standardGravitationalParameter);

    const orbital::OrbitalElements m_elements;
    const double m_energy{0.0};
};

} // namespace impl
} // namespace orbital

#endif //DAVIDIAN_ORBITIMPL_H
