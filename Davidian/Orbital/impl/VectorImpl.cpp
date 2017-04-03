//
// Created by alex on 4/3/17.
//

#include "VectorImpl.h"

namespace orbital{
namespace impl{

VectorImpl::VectorImpl(const double value0, const double value1, const double value2) : vector{value0, value1, value2} {}

VectorImpl::VectorImpl(const Array3D& array3D) : VectorImpl{array3D.at(0), array3D.at(1), array3D.at(2)}
{}

} // namespace impl
} // namespace orbital

//#ifdef BUILD_TESTS

#include <gtest/gtest.h>

class VectorImplTest : public ::testing::Test {
public:
  const orbital::Array3D expectedArray{{1.234, -5.679, 3.1415928}};
  const Eigen::Vector3d expectedVector{expectedArray.at(0), expectedArray.at(1), expectedArray.at(2)};
};

TEST_F(VectorImplTest, ThreeParameterCtor) {
  orbital::impl::VectorImpl vectorImpl{expectedArray.at(0), expectedArray.at(1), expectedArray.at(2)};
  EXPECT_EQ(expectedVector, vectorImpl.vector);
}

TEST_F(VectorImplTest, ArrayCtor) {
  orbital::impl::VectorImpl vectorImpl{expectedArray};
  EXPECT_EQ(expectedVector, vectorImpl.vector);
}

//#endif