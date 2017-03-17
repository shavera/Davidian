//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_SPHERICALVECTOR_H
#define DAVIDIAN_SPHERICALVECTOR_H

#include <memory>

namespace orbital{
class SphericalVector {
public:
  SphericalVector();
  SphericalVector(const double x, const double y, const double z);
  SphericalVector(const CartesianVector& otherVector);

  double const r();
  double const theta();
  double const phi();

private:
  class SphericalVectorImpl;
  std::unique_ptr<SphericalVectorImpl> m_impl;
};
} // namespace orbital

#endif //DAVIDIAN_SPHERICALVECTOR_H
