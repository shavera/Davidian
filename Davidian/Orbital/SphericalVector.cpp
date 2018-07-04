//
// Created by alex on 3/17/17.
//

#include "SphericalVector.h"
#include "CartesianVector.h"

namespace orbital{

namespace {

// Polar angle should only be defined between 0 and pi
double constrainPolarAngle(const double polar_angle){
  return std::fabs(std::remainder(polar_angle, 2*M_PI));
}

// Azimuthal angle should only be defined between -pi and pi
double constrainAzimuth(const double azimuth){
  return std::remainder(azimuth, 2*M_PI);
}

} // anonymous namespace

SphericalVector::SphericalVector()
    : m_vector{0, 0, 0}
{}

SphericalVector::SphericalVector(const double r, const double polar_angle, const double azimuth)
    : m_vector{r, constrainPolarAngle(polar_angle), constrainAzimuth(azimuth)}
{}

SphericalVector::SphericalVector(const CartesianVector& otherVector)
    : m_vector{[&otherVector](){
        const double norm{otherVector.norm()};
        const CartesianVector normalizedOther{otherVector.normalizedVector()};
        const double polarAngle{(0 == normalizedOther.z()) ? 0 : std::acos(normalizedOther.z())};
        return Eigen::Vector3d{norm, polarAngle, std::atan2(normalizedOther.y(), normalizedOther.x())};
      }()}
{}

double SphericalVector::r() const {
  return m_vector[0];
}

double SphericalVector::polarAngle() const {
  return m_vector[1];
}

double SphericalVector::azimuth() const {
  return m_vector[2];
}

double& SphericalVector::at(const size_t index) {
  return m_vector[index];
}

const double& SphericalVector::c_at(const size_t index) const {
  return m_vector[index];
}

} // namespace orbital