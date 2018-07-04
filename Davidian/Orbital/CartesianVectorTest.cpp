//
// Created by alexs on 7/3/18.
//

#include "CartesianVector.h"
#include "SphericalVector.h"

#include <gtest/gtest.h>

namespace orbital {

namespace {

class CartesianVectorTest : public ::testing::Test {

public:
  const double expectedX{1.2345}, expectedY{-2.341}, expectedZ{3.0};
  const double expectedNorm{std::sqrt(std::pow(expectedX, 2) + std::pow(expectedY, 2) + std::pow(expectedZ, 2))};
  CartesianVector vector{expectedX, expectedY, expectedZ};
};

TEST_F(CartesianVectorTest, DefaultConstructorTest) {
  CartesianVector defaultVector;
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

TEST_F(CartesianVectorTest, norm) {
  EXPECT_EQ(expectedNorm, vector.norm());
}

TEST_F(CartesianVectorTest, normalizedVector) {
  CartesianVector normalizedVector{vector.normalizedVector()};
  EXPECT_EQ(expectedX / expectedNorm, normalizedVector.x());
  EXPECT_EQ(expectedY / expectedNorm, normalizedVector.y());
  EXPECT_EQ(expectedZ / expectedNorm, normalizedVector.z());
}

TEST_F(CartesianVectorTest, at_indexAccessor) {
  double& xRef = vector.at(0);
  EXPECT_EQ(expectedX, xRef);
  double& yRef = vector.at(1);
  EXPECT_EQ(expectedY, yRef);
  double& zRef = vector.at(2);
  EXPECT_EQ(expectedZ, zRef);

  xRef = 23.1;
  EXPECT_EQ(23.1, vector.x());
}

TEST_F(CartesianVectorTest, c_at_indexAccessor) {
  EXPECT_EQ(expectedX, vector.c_at(0));
  EXPECT_EQ(expectedY, vector.c_at(1));
  EXPECT_EQ(expectedZ, vector.c_at(2));
}

TEST(CartesianVector_FromSpherical, simple) {
  const double expectedX{3.0}, expectedY{-4.0}, expectedZ{-5.0};
  const double r{5.0 * std::sqrt(2)};
  const double polarAngle{3 * M_PI / 4};
  const double azimuth{std::atan2(expectedY, expectedX)};

  EXPECT_NEAR(-.927295218, azimuth, 1e-5);

  const orbital::SphericalVector sphericalVector{r, polarAngle, azimuth};

  const CartesianVector actualVector{sphericalVector};
  EXPECT_NEAR(expectedX, actualVector.x(), 1e-12);
  EXPECT_NEAR(expectedY, actualVector.y(), 1e-12);
  EXPECT_NEAR(expectedZ, actualVector.z(), 1e-12);
}

TEST(CartesianVector_FromSpherical, northPoleSingularity) {
  const double expectedX{0.0}, expectedY{0.0}, expectedZ{5.0};
  const orbital::SphericalVector sphericalVector{5.0, 0.0, 1.2335};

  const CartesianVector cartesianVector{sphericalVector};

  EXPECT_EQ(expectedX, cartesianVector.x());
  EXPECT_EQ(expectedY, cartesianVector.y());
  EXPECT_EQ(expectedZ, cartesianVector.z());
}

TEST(CartesianVector_FromSpherical, southPoleSingularity) {
  const double expectedX{0.0}, expectedY{0.0}, expectedZ{-5.0};
  const orbital::SphericalVector sphericalVector{5.0, M_PI, -0.23235};

  const CartesianVector cartesianVector{sphericalVector};

  // because of M_PI being not precise for double, the south pole singularity doesn't exactly reproduce 0,0 coordinates
  EXPECT_NEAR(expectedX, cartesianVector.x(), 1e-15);
  EXPECT_NEAR(expectedY, cartesianVector.y(), 1e-15);
  EXPECT_EQ(expectedZ, cartesianVector.z());

}

TEST(CartesianVector_FromSpherical, originSingularity) {
  const orbital::SphericalVector sphericalVector;
  const CartesianVector cartesianVector{sphericalVector};

  EXPECT_EQ(0.0, cartesianVector.x());
  EXPECT_EQ(0.0, cartesianVector.y());
  EXPECT_EQ(0.0, cartesianVector.z());
}


class CartesianVectorOperationsTest : public ::testing::Test {
public:
  const CartesianVector leftVector{1.23, -3.24, 2.29};
  const CartesianVector rightVector{2.38, 4.22, -1.34};
};

TEST_F(CartesianVectorOperationsTest, dot) {
  const double expectedValue{-13.814};
  EXPECT_EQ(expectedValue, CartesianVector::dot(leftVector, rightVector));
  EXPECT_EQ(expectedValue, leftVector.dot(rightVector));
}

TEST_F(CartesianVectorOperationsTest, cross) {
  const CartesianVector expectedVector{-5.3222, 7.0984, 12.9018};
  CartesianVector actualVector{CartesianVector::cross(leftVector, rightVector)};
  EXPECT_NEAR(expectedVector.x(), actualVector.x(), 1e-4);
  EXPECT_NEAR(expectedVector.y(), actualVector.y(), 1e-4);
  EXPECT_NEAR(expectedVector.z(), actualVector.z(), 1e-4);

  actualVector = leftVector.cross(rightVector);
  EXPECT_NEAR(expectedVector.x(), actualVector.x(), 1e-4);
  EXPECT_NEAR(expectedVector.y(), actualVector.y(), 1e-4);
  EXPECT_NEAR(expectedVector.z(), actualVector.z(), 1e-4);
}

TEST_F(CartesianVectorOperationsTest, separation) {
  const CartesianVector differenceVector{.12, -.23, .34};
  const CartesianVector modifiedLeftVector{leftVector + differenceVector};
  EXPECT_NEAR(differenceVector.norm(), leftVector.separation(modifiedLeftVector), 1e-15);
}

TEST_F(CartesianVectorOperationsTest, negation_operator) {
  const CartesianVector negativeLeftVector{-1.23, 3.24, -2.29};
  EXPECT_EQ(negativeLeftVector, -leftVector);
}

TEST_F(CartesianVectorOperationsTest, addition_operator) {
  const CartesianVector expectedVector{1.23 + 2.38, -3.24 + 4.22, 2.29 - 1.34};
  EXPECT_EQ(expectedVector, leftVector + rightVector);
}

TEST_F(CartesianVectorOperationsTest, subtraction_operator) {
  const CartesianVector expectedVector{1.23 - 2.38, -3.24 - 4.22, 2.29 + 1.34};
  EXPECT_EQ(expectedVector, leftVector - rightVector);
}

TEST_F(CartesianVectorOperationsTest, equality_operator) {
  const CartesianVector congruentToLeftVector{1.23, -3.24, 2.29};
  EXPECT_TRUE(congruentToLeftVector == leftVector);
  EXPECT_FALSE(rightVector == congruentToLeftVector);
  EXPECT_EQ(leftVector, congruentToLeftVector);
}

TEST_F(CartesianVectorOperationsTest, inequality_operator) {
  const CartesianVector congruentToLeftVector{1.23, -3.24, 2.29};
  EXPECT_FALSE(congruentToLeftVector != leftVector);
  EXPECT_TRUE(rightVector != congruentToLeftVector);
  EXPECT_NE(rightVector, congruentToLeftVector);
}

TEST_F(CartesianVectorOperationsTest, multiplication_operator) {
  const CartesianVector initialVector{1, -2, 3.5}, expectedVector{14, -28, 49};
  const double factor{14};
  const CartesianVector vectorFromRight{initialVector * factor}, vectorFromLeft{factor * initialVector};
  EXPECT_EQ(expectedVector, vectorFromRight);
  EXPECT_EQ(expectedVector, vectorFromLeft);
}

TEST_F(CartesianVectorOperationsTest, division_operator) {
  const CartesianVector initialVector{39, -15.3, 64.2}, expectedVector{13, -5.1, 21.4};
  const double divisor{3};
  EXPECT_EQ(expectedVector, initialVector / divisor);
}

TEST_F(CartesianVectorOperationsTest, transformMatrix) {
  Eigen::Matrix3d rotationMatrix;
  rotationMatrix << 0.9790201665, 0.1788527114, 0.0976279731,
      -0.184745715, 0.9812450651, 0.0550194789,
      -0.0859565838, -0.0719015291, 0.9937009791;
  const CartesianVector initialVector{-1.8324404306, 1.2574985215, 0};
  const CartesianVector expectedVector{-1.56909, 1.57245, 0.0670943};
  CartesianVector actualVector = rotationMatrix * initialVector;
  for(int i{0}; i < 3; ++i){
    EXPECT_NEAR(expectedVector.c_at(i), actualVector.c_at(i), std::fabs(1e-6*expectedVector.c_at(i)));
  }
}

TEST_F(CartesianVectorOperationsTest, interpolate){
  const double t0{23.72}, t1{31.42}, t{26.43};

  const CartesianVector expectedVector{1.6347402631, -0.6144675328, 1.0124285716};

  const CartesianVector actualVector{interpolate(t0, leftVector, t1, rightVector, t)};

  EXPECT_GT(1e-6, expectedVector.separation(actualVector)) << actualVector;
}

} // anonymous namespace for testing
} // namespace orbital