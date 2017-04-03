//
// Created by alex on 3/17/17.
//

#include "CartesianVector.h"
#include "SphericalVector.h"

#include <Eigen/Dense>

namespace orbital{

namespace {
using VectorInitializer = std::array<double, 3>;

VectorInitializer createVectorFromSphericalCoords(const SphericalVector sphericalVector){
  const double& r = sphericalVector.r();
  const double& theta = sphericalVector.theta();
  const double& phi = sphericalVector.phi();
  return {r*sin(phi)*cos(theta), r*sin(phi)*sin(theta), r*cos(phi)};
}

} // anonymous namespace

class CartesianVector::Impl{
public:
  Impl(const double x, const double y, const double z) : vector{x,y,z} {}
  Impl(const VectorInitializer initializer) : Impl{initializer.at(0), initializer.at(1), initializer.at(2)} {}
  Eigen::Vector3d vector;
};

CartesianVector::CartesianVector() : m_impl{std::make_unique<CartesianVector::Impl>(0, 0, 0)} {}

CartesianVector::CartesianVector(const double x, const double y, const double z)
    : m_impl{std::make_unique<CartesianVector::Impl>(x,y,z)}
{}

CartesianVector::CartesianVector(const SphericalVector& otherVector)
    : m_impl{std::make_unique<CartesianVector::Impl>(createVectorFromSphericalCoords(otherVector))}
{}

double CartesianVector::x() const{
  return m_impl->vector.x();
}

double CartesianVector::y() const{
  return m_impl->vector.y();
}

double CartesianVector::z() const {
  return m_impl->vector.z();
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>


class CartesianVector : public ::testing::Test {
public:
  const double expectedX{1.2345}, expectedY{-2.341}, expectedZ{0.0};
  orbital::CartesianVector vector{expectedX, expectedY, expectedZ};
};

TEST_F(CartesianVector, x) {
  EXPECT_EQ(expectedX, vector.x());
}

TEST_F(CartesianVector, y) {
  EXPECT_EQ(expectedY, vector.y());
}

TEST_F(CartesianVector, z) {
  EXPECT_EQ(expectedZ, vector.z());
}


//#endif

