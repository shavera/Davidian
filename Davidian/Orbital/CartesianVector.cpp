//
// Created by alex on 3/17/17.
//

#include "CartesianVector.h"
#include "SphericalVector.h"

namespace orbital{

namespace {

Eigen::Vector3d createVectorFromSphericalCoords(const SphericalVector& sphericalVector){
  const double& r = sphericalVector.r();
  const double& polarAngle = sphericalVector.polarAngle();
  const double& azimuth = sphericalVector.azimuth();
  return {r*sin(polarAngle)*cos(azimuth), r*sin(polarAngle)*sin(azimuth), r*cos(polarAngle)};
}

} // anonymous namespace

CartesianVector::CartesianVector() : m_vector{0, 0, 0} {}

CartesianVector::CartesianVector(const double x, const double y, const double z)
    : m_vector{x,y,z}
{}

CartesianVector::CartesianVector(const SphericalVector& otherVector)
    : m_vector{createVectorFromSphericalCoords(otherVector)}
{}

double CartesianVector::x() const{
  return m_vector.x();
}

double CartesianVector::y() const{
  return m_vector.y();
}

double CartesianVector::z() const {
  return m_vector.z();
}

double CartesianVector::norm() const {
  return m_vector.norm();
}

CartesianVector CartesianVector::normalizedVector() const {
  CartesianVector normalizedVector;
  normalizedVector.m_vector = this->m_vector.normalized();
  return normalizedVector;
}

double CartesianVector::dot(const CartesianVector& vector1, const CartesianVector& vector2) {
  return vector1.m_vector.dot(vector2.m_vector);
}

CartesianVector CartesianVector::cross(const CartesianVector& leftVector, const CartesianVector& rightVector) {
  Eigen::Vector3d result = leftVector.m_vector.cross(rightVector.m_vector);
  CartesianVector returnVector;
  returnVector.m_vector = result;
  return returnVector;
}

double CartesianVector::dot(const CartesianVector& other) const {
  return CartesianVector::dot(*this, other);
}

CartesianVector CartesianVector::cross(const CartesianVector& rightVector) const {
  return CartesianVector::cross(*this, rightVector);
}

double CartesianVector::separation(const CartesianVector& other) const {
  return (*this - other).norm();
}

double& CartesianVector::at(size_t index) {
  return m_vector[index];
}

const double& CartesianVector::c_at(size_t index) const {
  return m_vector[index];
}

CartesianVector CartesianVector::operator-() const {
  CartesianVector negativeVector;
  negativeVector.m_vector = -(this->m_vector);
  return negativeVector;
}

CartesianVector CartesianVector::operator+(const CartesianVector& otherVector) const {
  CartesianVector sumVector;
  sumVector.m_vector = m_vector + otherVector.m_vector;
  return sumVector;
}

CartesianVector CartesianVector::operator-(const CartesianVector& otherVector) const {
  return *this + (-otherVector);
}

bool CartesianVector::operator==(const CartesianVector& otherVector) const {
  return m_vector.isApprox(otherVector.m_vector);
}

bool CartesianVector::operator!=(const CartesianVector& otherVector) const {
  return !(otherVector == *this);
}

CartesianVector CartesianVector::operator*(const double& scale) const {
  return CartesianVector{scale*this->x(), scale*this->y(), scale*this->z()};
}

CartesianVector operator*(const double& scale, const CartesianVector& vector){
  return CartesianVector{scale*vector.x(), scale*vector.y(), scale*vector.z()};
}

CartesianVector operator*(const Eigen::Matrix3d& transform, const CartesianVector& vector) {
  CartesianVector outputVector{};
  outputVector.m_vector = transform * vector.m_vector;
  return outputVector;
}

::std::ostream& operator<<(::std::ostream& os, const CartesianVector& v) {
  return os << "<" << v.x() << "," << v.y() << "," << v.z() <<">";
}

CartesianVector interpolate(double t0, CartesianVector v0, double t1, CartesianVector v1, double t) {
  const double deltaT{t1-t0};
  if(0 == deltaT){return CartesianVector{};}
  const double dT{t-t0};
  const double timeRatio{dT/deltaT};

  const auto difference = v1-v0;

  return v0 + (timeRatio*difference);
}

CartesianVector CartesianVector::operator/(const double& scale) const {
    CartesianVector scaledCopy{x()/scale, y()/scale, z()/scale};
    return scaledCopy;
}

CartesianVector::~CartesianVector() = default;

} // namespace orbital