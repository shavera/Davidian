//
// Created by alex on 3/17/17.
//

#include "SphericalVector.h"
#include "CartesianVector.h"

#include <Eigen/Dense>

namespace orbital{

namespace {

using VectorInitializer = std::array<double, 3>;

VectorInitializer createVectorFromCartesianCoords(const CartesianVector& cartesianVector){
  return {0,0,0};
};

} // anonymous namespace

class SphericalVector::Impl{
public:
  Impl(const double r, const double polar, const double azimuth){}
  Impl(const VectorInitializer initializer) : Impl{initializer.at(0), initializer.at(1), initializer.at(2)} {}
  Eigen::Vector3d vector;
};

SphericalVector::SphericalVector()
    : m_impl{std::make_unique<SphericalVector::Impl>(0, 0, 0)}  {

}

SphericalVector::SphericalVector(const double r, const double polar_angle, const double azimuth)
    : m_impl{std::make_unique<SphericalVector::Impl>(r, polar_angle, azimuth)}
{}

SphericalVector::SphericalVector(const CartesianVector& otherVector)
    : m_impl{std::make_unique<SphericalVector::Impl>(createVectorFromCartesianCoords(otherVector))}
{}

double SphericalVector::r() const {
  return 0;
}

double SphericalVector::polar_angle() const {
  return 0;
}

double SphericalVector::azimuth() const {
  return 0;
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>


class OrbitalVectorTest : public ::testing::Test {
public:
  const double expectedR{222.222}, expectedPolarAngle{1.23}, expectedAzimuth{-0.12};
  orbital::SphericalVector vector{expectedR, expectedPolarAngle, expectedAzimuth};
};

TEST_F(OrbitalVectorTest, r) {
  EXPECT_EQ(expectedR, vector.r());
}

TEST_F(OrbitalVectorTest, polar_angle) {
  EXPECT_EQ(expectedPolarAngle, vector.polar_angle());
}

TEST_F(OrbitalVectorTest, azimuth) {
  EXPECT_EQ(expectedAzimuth, vector.azimuth());
}


//#endif