//
// Created by alex on 4/3/17.
//

#ifndef DAVIDIAN_VECTORIMPL_H
#define DAVIDIAN_VECTORIMPL_H

#include <Eigen/Dense>

namespace orbital {

using Array3D = std::array<double, 3>;

namespace impl{

struct VectorImpl {
  VectorImpl(const double value0, const double value1, const double value2);
  VectorImpl(const Array3D& array3D);
  Eigen::Vector3d vector;
};

} // namespace impl
} // namespace orbital

#endif //DAVIDIAN_VECTORIMPL_H
