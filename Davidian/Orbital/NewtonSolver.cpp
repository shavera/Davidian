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
