//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_ORBIT_H
#define DAVIDIAN_ORBIT_H

#include "impl/GlobalDefinitions.h"

#include <memory>

namespace orbital{

namespace impl{
class OrbitImpl;
} // namespace impl




class Orbit {
public:
    Orbit(const StateVector& stateVector, const double barymass, const double leptomass, impl::OrbitImpl* impl);
    virtual ~Orbit();

private:
    std::unique_ptr<impl::OrbitImpl> m_impl;
};

} // namespace orbital


#endif //DAVIDIAN_ORBIT_H
