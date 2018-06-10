//
// Created by Alex Shaver on 10/8/17.
//
#include "NewtonSolver.h"
#include <cmath>

namespace engine {

double findFunctionRoot(const std::function<double(double)>& f,
                        const std::function<double(double)>& derivative,
                        double x0,
                        double tolerance) {
  double x{x0}, lastX, limit;
  do {
    lastX = x;
    x = x - f(x) / derivative(x);
    limit = (0==lastX) ? tolerance : tolerance*lastX;
  } while(limit < std::fabs(lastX - x));
  return x;
}

} // namespace engine

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace {

TEST(EquationSolver, solveFunction){
  auto testFunction = [](double x){return std::log(x+0.2) + std::pow(x, 3) + 0.908290732;};
  auto testDerivative = [](double x){return 1/(x+0.2) + 3*std::pow(x,2);};
  const double expectedValue{0.2};
  const double actualValue{findFunctionRoot(testFunction, testDerivative, 0)};

  EXPECT_NEAR(expectedValue, actualValue, 1e-6*expectedValue);
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS