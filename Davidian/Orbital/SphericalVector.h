//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_SPHERICALVECTOR_H
#define DAVIDIAN_SPHERICALVECTOR_H

#include <memory>

namespace orbital{

class CartesianVector;

class SphericalVector {
public:
  SphericalVector();
  explicit SphericalVector(const double r, const double polar_angle, const double azimuth);
  SphericalVector(const CartesianVector& otherVector);

  double r() const;
  double polar_angle() const;
  double azimuth() const;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

} // namespace orbital

#endif //DAVIDIAN_SPHERICALVECTOR_H
