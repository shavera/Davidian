//
// Created by Alex Shaver on 10/8/17.
//
#include "NewtonSolver.h"
#include <cmath>

using namespace std::placeholders;

namespace orbital {

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

namespace{
double eccentricAnomaly_f(double meanAnomaly, double eccentricity, double eccentricAnomaly){
  return eccentricAnomaly - eccentricity*std::sin(eccentricAnomaly) - meanAnomaly;
}

double eccentricAnomaly_d(double eccentricity, double eccentricAnomaly){
  return 1 - eccentricity*std::cos(eccentricAnomaly);
}
} // anonymous namespace

double findEccentricAnomaly(double meanAnomaly, double eccentricity)
{
  return findFunctionRoot(std::bind(&eccentricAnomaly_f, meanAnomaly, eccentricity, _1),
                          std::bind(&eccentricAnomaly_d, eccentricity, _1),
                          meanAnomaly);
}

} // namespace orbital

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

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

#endif // BUILD_TESTS
