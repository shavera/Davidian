//
// Created by alexs on 7/3/18.
//

#include "NewtonSolver.h"

#include <gtest/gtest.h>

#include <cmath>

namespace orbital{
namespace {

TEST(EquationSolver, solveFunction){
  auto testFunction = [](double x){return std::log(x+0.2) + std::pow(x, 3) + 0.908290732;};
  auto testDerivative = [](double x){return 1/(x+0.2) + 3*std::pow(x,2);};
  const double expectedValue{0.2};
  const double actualValue{findFunctionRoot(testFunction, testDerivative, 0)};

  EXPECT_NEAR(expectedValue, actualValue, 1e-6*expectedValue);
}


TEST(EccentricAnomalySolver, eccentricAnomaly){
  const double meanAnomaly{0.71163115893}, eccentricity{0.55};
  const double expectedEccentricAnomaly{1.23}, actualEccentricAnomaly{findEccentricAnomaly(meanAnomaly, eccentricity)};

  EXPECT_NEAR(expectedEccentricAnomaly, actualEccentricAnomaly, 1e-6*expectedEccentricAnomaly);
}

} // anonymous namespace
} // namespace orbital
