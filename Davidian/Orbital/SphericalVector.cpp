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
  Impl(const double r, const double theta, const double phi){}
  Impl(const VectorInitializer initializer) : Impl{initializer.at(0), initializer.at(1), initializer.at(2)} {}
  Eigen::Vector3d vector;
};

SphericalVector::SphericalVector()
    : m_impl{std::make_unique<SphericalVector::Impl>(0, 0, 0)}  {

}

SphericalVector::SphericalVector(const double r, const double theta, const double phi)
    : m_impl{std::make_unique<SphericalVector::Impl>(r, theta, phi)}
{}

SphericalVector::SphericalVector(const CartesianVector& otherVector)
    : m_impl{std::make_unique<SphericalVector::Impl>(createVectorFromCartesianCoords(otherVector))}
{}

double SphericalVector::r() const {
  return 0;
}

double SphericalVector::theta() const {
  return 0;
}

double SphericalVector::phi() const {
  return 0;
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>


class OrbitalVectorTest : public ::testing::Test {
public:
  const double expectedR{222.222}, expectedTheta{1.23}, expectedPhi{-0.12};
  orbital::SphericalVector vector{expectedR, expectedTheta, expectedPhi};
};

TEST_F(OrbitalVectorTest, r) {
  EXPECT_EQ(expectedR, vector.r());
}

TEST_F(OrbitalVectorTest, theta) {
  EXPECT_EQ(expectedTheta, vector.theta());
}

TEST_F(OrbitalVectorTest, phi) {
  EXPECT_EQ(expectedPhi, vector.phi());
}


//#endif