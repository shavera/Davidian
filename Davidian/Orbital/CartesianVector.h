//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_CARTESIANVECTOR_H
#define DAVIDIAN_CARTESIANVECTOR_H

#include <memory>

namespace orbital{

class SphericalVector;

class CartesianVector {
public:
  CartesianVector();
  CartesianVector(double x, double y, double z);
  CartesianVector(const SphericalVector& otherVector);

  double x();
  double y();
  double z();

private:
  class CartesianVectorImpl;
  std::unique_ptr<CartesianVectorImpl> m_impl;
};

} // namespace orbital


#endif //DAVIDIAN_CARTESIANVECTOR_H
