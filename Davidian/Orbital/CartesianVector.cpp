//
// Created by alex on 3/17/17.
//

#include "CartesianVector.h"
#include "SphericalVector.h"
#include "impl/VectorImpl.h"

#include <Eigen/Dense>

namespace orbital{

namespace {

Array3D createVectorFromSphericalCoords(const SphericalVector& sphericalVector){
  const double& r = sphericalVector.r();
  const double& theta = sphericalVector.polar_angle();
  const double& phi = sphericalVector.azimuth();
  return {r*sin(phi)*cos(theta), r*sin(phi)*sin(theta), r*cos(phi)};
}

} // anonymous namespace

CartesianVector::CartesianVector() : m_impl{std::make_unique<impl::VectorImpl>(0, 0, 0)} {}

CartesianVector::CartesianVector(const double x, const double y, const double z)
    : m_impl{std::make_unique<impl::VectorImpl>(x,y,z)}
{}

CartesianVector::CartesianVector(const SphericalVector& otherVector)
    : m_impl{std::make_unique<impl::VectorImpl>(createVectorFromSphericalCoords(otherVector))}
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

double CartesianVector::norm() const {
  return 0;
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace {

class CartesianVectorTest : public ::testing::Test {
public:
  const double expectedX{1.2345}, expectedY{-2.341}, expectedZ{0.0};
  orbital::CartesianVector vector{expectedX, expectedY, expectedZ};
};

TEST_F(CartesianVectorTest, DefaultConstructorTest) {
  orbital::CartesianVector defaultVector;
  EXPECT_EQ(0, defaultVector.x());
  EXPECT_EQ(0, defaultVector.y());
  EXPECT_EQ(0, defaultVector.z());
}

TEST_F(CartesianVectorTest, x) {
  EXPECT_EQ(expectedX, vector.x());
}

TEST_F(CartesianVectorTest, y) {
  EXPECT_EQ(expectedY, vector.y());
}

TEST_F(CartesianVectorTest, z) {
  EXPECT_EQ(expectedZ, vector.z());
}

} // anonymous namespace for testing

//#endif

