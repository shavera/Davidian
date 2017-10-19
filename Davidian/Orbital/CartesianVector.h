//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_CARTESIANVECTOR_H
#define DAVIDIAN_CARTESIANVECTOR_H

#include <Eigen/Dense>

namespace orbital{

class SphericalVector;

class CartesianVector {
public:
  CartesianVector();
  CartesianVector(const double x, const double y, const double z);
  CartesianVector(const SphericalVector& otherVector);
  virtual ~CartesianVector();

  double x() const;
  double y() const;
  double z() const;

  double norm() const;
  CartesianVector normalizedVector() const;

  static double dot(const CartesianVector& vector1, const CartesianVector& vector2);
  static CartesianVector cross(const CartesianVector& leftVector, const CartesianVector& rightVector);
  double dot(const CartesianVector& other) const;
  CartesianVector cross(const CartesianVector& rightVector) const;

  /// magnitude of the difference between two vectors
  double separation(const CartesianVector& other) const;

  /// bounds checking member access by 0-based index (0->x, 1->y, 2->z)
  double& at(size_t index);
  const double& c_at(size_t index) const;

  CartesianVector operator-() const;
  CartesianVector operator+(const CartesianVector& otherVector) const;
  CartesianVector operator-(const CartesianVector& otherVector) const;
  CartesianVector operator*(const double& scale) const;
  CartesianVector operator/(const double& scale) const;
  bool operator==(const CartesianVector& otherVector) const;
  bool operator!=(const CartesianVector& otherVector) const;

private:
  Eigen::Vector3d m_vector;
};

CartesianVector operator*(const double& factor, const CartesianVector& vector);

} // namespace orbital


#endif //DAVIDIAN_CARTESIANVECTOR_H
