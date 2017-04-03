//
// Created by alex on 3/17/17.
//

#include "SphericalVector.h"
#include "CartesianVector.h"
#include "impl/VectorImpl.h"

#include <Eigen/Dense>

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
    : m_impl{std::make_unique<impl::VectorImpl>(0, 0, 0)}  {

}

SphericalVector::SphericalVector(const double r, const double polar_angle, const double azimuth)
    : m_impl{std::make_unique<impl::VectorImpl>(r,
                                                constrainPolarAngle(polar_angle),
                                                constrainAzimuth(azimuth))}
{}

SphericalVector::SphericalVector(const CartesianVector& otherVector)
    : m_impl{std::make_unique<impl::VectorImpl>([&otherVector](){
      const double norm{otherVector.m_impl->vector.norm()};
      return Array3D{norm, 0, 0};
      }()
    )}
{}

double SphericalVector::r() const {
  return m_impl->vector[0];
}

double SphericalVector::polar_angle() const {
  return m_impl->vector[1];
}

double SphericalVector::azimuth() const {
  return m_impl->vector[2];
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace {
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

// first is actual (times pi) and second is expected
using AngleDataPair = std::pair<double, double>;

class OrbitalVector_AngleWrappingTest : public OrbitalVectorTest, public ::testing::WithParamInterface< AngleDataPair > {
public:
  void SetUp() {
    testData = GetParam();
    maxAllowedError = std::fabs(testData.second/1e12);
  }
  AngleDataPair testData;
  double maxAllowedError;
};

class OrbitalVector_PolarWrappingTest : public OrbitalVector_AngleWrappingTest{};

std::array<AngleDataPair, 8> polarAngleData{{
                                                {0,0},
                                                {0.25, 0.25},
                                                {-0.25, 0.25},
                                                {4.5, 0.5},
                                                {3.5, 0.5},
                                                {4.75, 0.75},
                                                {3.25, 0.75},
                                                {3.75, 0.25}
                                            }};

TEST_P(OrbitalVector_PolarWrappingTest, polarAngle){
  const double polarAngle = testData.first * M_PI;
  orbital::SphericalVector _vector{expectedR, polarAngle, expectedAzimuth};
  EXPECT_NEAR(testData.second, _vector.polar_angle()/M_PI, maxAllowedError);
}

INSTANTIATE_TEST_CASE_P(OrbitalVector_PolarWrappingTest,
                        OrbitalVector_PolarWrappingTest, ::testing::ValuesIn(polarAngleData));

class OrbitalVector_AzimuthWrappingTest : public OrbitalVector_AngleWrappingTest{};

std::array<AngleDataPair, 10> azimuthData{{
    {0, 0},
    {0.25, 0.25},
    {-0.25, -0.25},
    {4.5, 0.5},
    {3.5, -0.5},
    {4.75, 0.75},
    {3.25, -0.75},
    {-3.25, 0.75},
    {1, 1},
    {-1, -1}
}};

TEST_P(OrbitalVector_AzimuthWrappingTest, azimuth) {
  const double azimuth = testData.first * M_PI;
  orbital::SphericalVector _vector{expectedR, expectedPolarAngle, azimuth};
  EXPECT_NEAR(testData.second, _vector.azimuth()/M_PI, maxAllowedError);
}

INSTANTIATE_TEST_CASE_P(OrbitalVector_AzimuthWrappingTest,
                        OrbitalVector_AzimuthWrappingTest, ::testing::ValuesIn(azimuthData));

} // anonymous namespace for testing

//#endif