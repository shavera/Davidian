//
// Created by alex on 3/17/17.
//

#ifndef DAVIDIAN_SPHERICALVECTOR_H
#define DAVIDIAN_SPHERICALVECTOR_H

#include <memory>

namespace orbital{

namespace impl{
class VectorImpl;
}

class CartesianVector;

class SphericalVector {
public:
  SphericalVector();
  /**
   * Construct spherical vector from parameters.
   * This constructor does very minor and likely buggy data sanitization. See parameters for details
   *
   * @param r radius; if negative, will use absolute value. Won't change angle values when the sign flips, however.
   * @param polar_angle; angle, in radians, from the positive z axis. Will be forced to between 0 and pi. Should the angle be outside those bounds, the angle will be adjusted to fall within them, but the azimuthal angle will not change.
   * @param azimuth; angle, in radians, from the positive x axis. Will be forced to between -pi and pi. This will just be a simple normalization and shouldn't impact the other two parameters anyway.
   */
  explicit SphericalVector(const double r, const double polar_angle, const double azimuth);
  SphericalVector(const CartesianVector& otherVector);

  double r() const;
  double polarAngle() const;
  double azimuth() const;

  /// index based accessor, 0->r, 1->polarAngle, 2->azimuth.
  /// WARNING! if you set a new value using at, there's no angle wrapping involved, so there could be issues associated.
  double& at(const size_t index);
  const double& c_at(const size_t index) const;

private:
  std::unique_ptr<impl::VectorImpl> m_impl;
};

} // namespace orbital

#endif //DAVIDIAN_SPHERICALVECTOR_H
