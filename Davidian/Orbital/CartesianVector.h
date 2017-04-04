//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_CARTESIANVECTOR_H
#define DAVIDIAN_CARTESIANVECTOR_H

#include <memory>

namespace orbital{

namespace impl{
class VectorImpl;
}

class SphericalVector;

class CartesianVector {
public:
  CartesianVector();
  explicit CartesianVector(const double x, const double y, const double z);
  CartesianVector(const SphericalVector& otherVector);

  double x() const;
  double y() const;
  double z() const;

  double norm() const;
  CartesianVector normalizedVector() const;

private:
  std::unique_ptr<impl::VectorImpl> m_impl;
};

} // namespace orbital


#endif //DAVIDIAN_CARTESIANVECTOR_H
