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
  const double& polarAngle = sphericalVector.polarAngle();
  const double& azimuth = sphericalVector.azimuth();
  return {r*sin(polarAngle)*cos(azimuth), r*sin(polarAngle)*sin(azimuth), r*cos(polarAngle)};
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
  return m_impl->vector.norm();
}

CartesianVector CartesianVector::normalizedVector() const {
  CartesianVector normalizedVector;
  normalizedVector.m_impl->vector = this->m_impl->vector.normalized();
  return normalizedVector;
}

double CartesianVector::dot(const CartesianVector& vector1, const CartesianVector& vector2) {
  return vector1.m_impl->vector.dot(vector2.m_impl->vector);
}

CartesianVector CartesianVector::cross(const CartesianVector& leftVector, const CartesianVector& rightVector) {
  Eigen::Vector3d result = leftVector.m_impl->vector.cross(rightVector.m_impl->vector);
  CartesianVector returnVector;
  returnVector.m_impl->vector = result;
  return returnVector;
}

double CartesianVector::dot(const CartesianVector& other) const {
  return CartesianVector::dot(*this, other);
}

CartesianVector CartesianVector::cross(const CartesianVector& rightVector) const {
  return CartesianVector::cross(*this, rightVector);
}

CartesianVector CartesianVector::operator-() const {
  return CartesianVector();
}

CartesianVector CartesianVector::operator+(const CartesianVector& otherVector) const {
  return CartesianVector();
}

CartesianVector CartesianVector::operator-(const CartesianVector& otherVector) const {
  return CartesianVector();
}

bool CartesianVector::operator==(const CartesianVector& otherVector) const {
  return m_impl->vector == otherVector.m_impl->vector;
}

bool CartesianVector::operator!=(const CartesianVector& otherVector) const {
  return !(otherVector == *this);
}

} // namespace orbital

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace {

class CartesianVectorTest : public ::testing::Test {
public:
  const double expectedX{1.2345}, expectedY{-2.341}, expectedZ{3.0};
  const double expectedNorm{std::sqrt(std::pow(expectedX, 2) + std::pow(expectedY,2 ) + std::pow(expectedZ, 2))};
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

TEST_F(CartesianVectorTest, norm){
  EXPECT_EQ(expectedNorm, vector.norm());
}

TEST_F(CartesianVectorTest, normalizedVector){
  orbital::CartesianVector normalizedVector{vector.normalizedVector()};
  EXPECT_EQ(expectedX/expectedNorm, normalizedVector.x());
  EXPECT_EQ(expectedY/expectedNorm, normalizedVector.y());
  EXPECT_EQ(expectedZ/expectedNorm, normalizedVector.z());
}

TEST(CartesianVector_FromSpherical, simple){
  const double expectedX{3.0}, expectedY{-4.0}, expectedZ{-5.0};
  const double r{5.0*std::sqrt(2)};
  const double polarAngle{3*M_PI/4};
  const double azimuth{std::atan2(expectedY, expectedX)};

  EXPECT_NEAR(-.927295218, azimuth, 1e-5);

  const orbital::SphericalVector sphericalVector{r, polarAngle, azimuth};

  const orbital::CartesianVector actualVector{sphericalVector};
  EXPECT_NEAR(expectedX, actualVector.x(), 1e-12);
  EXPECT_NEAR(expectedY, actualVector.y(), 1e-12);
  EXPECT_NEAR(expectedZ, actualVector.z(), 1e-12);
}

TEST(CartesianVector_FromSpherical, northPoleSingularity){
  const double expectedX{0.0}, expectedY{0.0}, expectedZ{5.0};
  const orbital::SphericalVector sphericalVector{5.0, 0.0, 1.2335};

  const orbital::CartesianVector cartesianVector{sphericalVector};

  EXPECT_EQ(expectedX, cartesianVector.x());
  EXPECT_EQ(expectedY, cartesianVector.y());
  EXPECT_EQ(expectedZ, cartesianVector.z());
}

TEST(CartesianVector_FromSpherical, southPoleSingularity){
  const double expectedX{0.0}, expectedY{0.0}, expectedZ{-5.0};
  const orbital::SphericalVector sphericalVector{5.0, M_PI, -0.23235};

  const orbital::CartesianVector cartesianVector{sphericalVector};

  // because of M_PI being not precise for double, the south pole singularity doesn't exactly reproduce 0,0 coordinates
  EXPECT_NEAR(expectedX, cartesianVector.x(), 1e-15);
  EXPECT_NEAR(expectedY, cartesianVector.y(), 1e-15);
  EXPECT_EQ(expectedZ, cartesianVector.z());

}

TEST(CartesianVector_FromSpherical, originSingularity){
  const orbital::SphericalVector sphericalVector;
  const orbital::CartesianVector cartesianVector{sphericalVector};

  EXPECT_EQ(0.0, cartesianVector.x());
  EXPECT_EQ(0.0, cartesianVector.y());
  EXPECT_EQ(0.0, cartesianVector.z());
}


class CartesianVectorOperationsTest : public ::testing::Test {
public:
  orbital::CartesianVector leftVector{1.23, -3.24, 2.29};
  orbital::CartesianVector rightVector{2.38, 4.22, -1.34};
};

TEST_F(CartesianVectorOperationsTest, dot){
  const double expectedValue{-13.814};
  EXPECT_EQ(expectedValue, orbital::CartesianVector::dot(leftVector, rightVector));
  EXPECT_EQ(expectedValue, leftVector.dot(rightVector));
}

TEST_F(CartesianVectorOperationsTest, cross){
  const orbital::CartesianVector expectedVector{-5.3222, 7.0984, 12.9018};
  orbital::CartesianVector actualVector{orbital::CartesianVector::cross(leftVector, rightVector)};
  EXPECT_NEAR(expectedVector.x(), actualVector.x(), 1e-4);
  EXPECT_NEAR(expectedVector.y(), actualVector.y(), 1e-4);
  EXPECT_NEAR(expectedVector.z(), actualVector.z(), 1e-4);

  actualVector = leftVector.cross(rightVector);
  EXPECT_NEAR(expectedVector.x(), actualVector.x(), 1e-4);
  EXPECT_NEAR(expectedVector.y(), actualVector.y(), 1e-4);
  EXPECT_NEAR(expectedVector.z(), actualVector.z(), 1e-4);
}
/*
TEST_F(CartesianVectorOperationsTest, separation){

}

TEST_F(CartesianVectorOperationsTest, negation_operator){

}

TEST_F(CartesianVectorOperationsTest, addition_operator){
//  const orbital::CartesianVector expecteVector{};
}

TEST_F(CartesianVectorOperationsTest, subtraction_operator){

}
*/
TEST_F(CartesianVectorOperationsTest, equality_operator){
  const orbital::CartesianVector congruentToLeftVector{1.23, -3.24, 2.29};
  EXPECT_TRUE(congruentToLeftVector == leftVector);
  EXPECT_FALSE(rightVector == congruentToLeftVector);
  EXPECT_EQ(leftVector, congruentToLeftVector);
}

TEST_F(CartesianVectorOperationsTest, inequality_operator){
  const orbital::CartesianVector congruentToLeftVector{1.23, -3.24, 2.29};
  EXPECT_FALSE(congruentToLeftVector != leftVector);
  EXPECT_TRUE(rightVector != congruentToLeftVector);
  EXPECT_NE(rightVector, congruentToLeftVector);
}

} // anonymous namespace for testing

#endif

