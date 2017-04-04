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
      const double norm{otherVector.norm()};
      const CartesianVector normalizedOther{otherVector.normalizedVector()};
      const double polarAngle{(0 == normalizedOther.z()) ? 0 : std::acos(normalizedOther.z())};
      return Array3D{norm, polarAngle, std::atan2(normalizedOther.y(), normalizedOther.x())};
      }()
    )}
{}

double SphericalVector::r() const {
  return m_impl->vector[0];
}

double SphericalVector::polarAngle() const {
  return m_impl->vector[1];
}

double SphericalVector::azimuth() const {
  return m_impl->vector[2];
}

} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace {

class SphericalVectorTest : public ::testing::Test {
public:
  const double expectedR{222.222}, expectedPolarAngle{1.23}, expectedAzimuth{-0.12};
  orbital::SphericalVector vector{expectedR, expectedPolarAngle, expectedAzimuth};
};

TEST_F(SphericalVectorTest, defaultConstructorTest){
  orbital::SphericalVector _vector;
  EXPECT_EQ(0, _vector.r());
  EXPECT_EQ(0, _vector.polarAngle());
  EXPECT_EQ(0, _vector.azimuth());
}

TEST_F(SphericalVectorTest, r) {
  EXPECT_EQ(expectedR, vector.r());
}

TEST_F(SphericalVectorTest, polar_angle) {
  EXPECT_EQ(expectedPolarAngle, vector.polarAngle());
}

TEST_F(SphericalVectorTest, azimuth){
  EXPECT_EQ(expectedAzimuth, vector.azimuth());
}

// first is actual (times pi) and second is expected
using AngleDataPair = std::pair<double, double>;

class SphericalVector_AngleWrappingTest : public SphericalVectorTest, public ::testing::WithParamInterface< AngleDataPair > {
public:
  void SetUp() {
    testData = GetParam();
    maxAllowedError = std::fabs(testData.second/1e12);
  }
  AngleDataPair testData;
  double maxAllowedError;
};

class SphericalVector_PolarWrappingTest : public SphericalVector_AngleWrappingTest{};

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

TEST_P(SphericalVector_PolarWrappingTest, polarAngle){
  const double polarAngle = testData.first * M_PI;
  orbital::SphericalVector _vector{expectedR, polarAngle, expectedAzimuth};
  EXPECT_NEAR(testData.second, _vector.polarAngle()/M_PI, maxAllowedError);
}

INSTANTIATE_TEST_CASE_P(SphericalVector_PolarWrappingTest,
                        SphericalVector_PolarWrappingTest, ::testing::ValuesIn(polarAngleData));

class SphericalVector_AzimuthWrappingTest : public SphericalVector_AngleWrappingTest{};

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

TEST_P(SphericalVector_AzimuthWrappingTest, azimuth) {
  const double azimuth = testData.first * M_PI;
  orbital::SphericalVector _vector{expectedR, expectedPolarAngle, azimuth};
  EXPECT_NEAR(testData.second, _vector.azimuth()/M_PI, maxAllowedError);
}

INSTANTIATE_TEST_CASE_P(SphericalVector_AzimuthWrappingTest,
                        SphericalVector_AzimuthWrappingTest, ::testing::ValuesIn(azimuthData));

TEST(SphericalVector_FromCartesian, SimpleConversion){
  const double expectedX{1.2345}, expectedY{-2.341}, expectedZ{3.0};
  const orbital::CartesianVector vector{expectedX, expectedY, expectedZ};
  const double expectedNorm{std::sqrt(std::pow(expectedX, 2) + std::pow(expectedY,2 ) + std::pow(expectedZ, 2))};
  const orbital::CartesianVector normalizedVector{expectedX/expectedNorm, expectedY/expectedNorm, expectedZ/expectedNorm};
  const double expectedPolarAngle{std::acos(normalizedVector.z())};
  const double expectedAzimuth{std::atan2(normalizedVector.y(), normalizedVector.x())};

  std::cout << normalizedVector.x() << " " << normalizedVector.y() << " " << normalizedVector.z() << std::endl;

  const orbital::SphericalVector actualVector{vector};

  std::cout << actualVector.r() <<" " << actualVector.polarAngle() << " " << actualVector.azimuth() << std::endl;

  /* Cross check with hand calculated numbers if necessary:
   * x,y,z: 1.2345, -2.341, 3.0 -> 4.00053, .7228854, -1.0855175

  EXPECT_NEAR(4.00053, actualVector.r(), 1e-5);
  EXPECT_NEAR(.7228854, actualVector.polarAngle(), 1e-5);
  EXPECT_NEAR(-1.0855175, actualVector.azimuth(), 1e-5);
  */

  EXPECT_NEAR(expectedNorm, actualVector.r(), 1e-12);
  EXPECT_NEAR(expectedPolarAngle, actualVector.polarAngle(), 1e-12);
  EXPECT_NEAR(expectedAzimuth, actualVector.azimuth(), 1e-12);
}

TEST(SphericalVector_FromCartesian, PolarSingularities){
  const orbital::CartesianVector northPole{0, 0, 5};
  orbital::SphericalVector testVector;
  ASSERT_NO_THROW(testVector = orbital::SphericalVector{northPole});
  EXPECT_EQ(5, testVector.r());
  EXPECT_EQ(0, testVector.polarAngle());
  EXPECT_EQ(0, testVector.azimuth());
}

TEST(SphericalVector_FromCartesian, SouthPoleSingularity){
  const orbital::CartesianVector southPole{0, 0, -5};
  orbital::SphericalVector testVector;
  ASSERT_NO_THROW(testVector = orbital::SphericalVector{southPole});
  EXPECT_EQ(5, testVector.r());
  EXPECT_EQ(M_PI, testVector.polarAngle());
  EXPECT_EQ(0, testVector.azimuth());
}

TEST(SphericalVector_FromCartesian, OriginSingularity){
  const orbital::CartesianVector zeroVector{0, 0, 0};
  orbital::SphericalVector testVector;
  ASSERT_NO_THROW(testVector = orbital::SphericalVector{zeroVector});
  EXPECT_EQ(0, testVector.r());
  EXPECT_EQ(0, testVector.polarAngle());
  EXPECT_EQ(0, testVector.azimuth());

}

} // anonymous namespace for testing

//#endif