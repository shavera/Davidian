//
// Created by alex on 3/17/17.
//

#include "Orbit.h"

#include "NewtonSolver.h"

namespace orbital {
namespace {

double standardGravitationalParameter_f(double m1, double m2) {
  return orbital::G*(m1 + m2);
}

double energy_f(const StateVector& stateVector, const double standardGravitationalParameter){
  return stateVector.velocity.dot(stateVector.velocity)/2 - standardGravitationalParameter/stateVector.position.norm();
}

double energy_f(const double semiMajorAxis, const double standardGravitationalParamter){
  return (semiMajorAxis == 0) ? 0 : -standardGravitationalParamter/(2*semiMajorAxis);
}

CartesianVector specificAngularMomentum_f(const StateVector& stateVector){
  CartesianVector angularMomentum = stateVector.position.cross(stateVector.velocity);
  return angularMomentum;
}

double eccentricity_f(const double specificOrbitalEnergy, const double standardGravitationalParameter, const CartesianVector& specificAngularMomentum){
  return sqrt(1 + (2*specificOrbitalEnergy*specificAngularMomentum.dot(specificAngularMomentum))/(std::pow(standardGravitationalParameter, 2)));
}

double semiMajorAxis_f(const double standardGravitationalParameter, const double specificOrbitalEnergy){
  return -standardGravitationalParameter/(2*specificOrbitalEnergy);
}

double period_f(double semiMajorAxis, double standardGravitationalParameter) {
  return 2*M_PI*std::sqrt(std::pow(semiMajorAxis, 3)/standardGravitationalParameter);
}

double sweep_f(double period) {
  return 2*M_PI/period;
}

double inclination_f(const CartesianVector& specificAngularMomentum){
  return std::acos(specificAngularMomentum.z()/specificAngularMomentum.norm());
}

CartesianVector vectorOfAscendingNode_f(const CartesianVector& specificAngularMomentum){
  return CartesianVector{-specificAngularMomentum.y(), specificAngularMomentum.x(), 0};
}

double longitudeOfAscendingNode_f(const CartesianVector& vectorOfAscendingNode){
  return std::atan2(vectorOfAscendingNode.y(), vectorOfAscendingNode.x());
}

CartesianVector eccentricityVector_f(const StateVector& stateVector, const CartesianVector& specificAngularMomentum, const double standardGravitationalParameter){
  return stateVector.velocity.cross(specificAngularMomentum)/standardGravitationalParameter - stateVector.position.normalizedVector();
}

double argumentOfPeriapsis_f(const CartesianVector& ascendingNodeVector, const CartesianVector& eccentricityVector){
  if(CartesianVector{0,0,0} == ascendingNodeVector){return std::atan2(eccentricityVector.y(), eccentricityVector.x());}

  double candidateValue{ std::acos(ascendingNodeVector.normalizedVector().dot(eccentricityVector.normalizedVector())) };
  return (eccentricityVector.z() > 0) ? candidateValue : 2*M_PI - candidateValue;
}

double eccentricAnomaly_f(double radius, double semiMajorAxis, double eccentricity){
  return (0==eccentricity) ? 0.0 : std::acos((1-radius/semiMajorAxis)/eccentricity);
}

double meanAnomaly_f(double eccentricAnomaly, double eccentricity){
  return eccentricAnomaly - eccentricity * std::sin(eccentricAnomaly);
}

double angularMomentumMagnitudeSquared_f(double stdGravParam, double semiMajorAxis, double eccentricity){
  return stdGravParam*semiMajorAxis*(1-(std::pow(eccentricity, 2)));
}

double radius_f(double semiMajorAxis, double eccentricity, double eccentricAnomaly){
  return semiMajorAxis*(1-eccentricity*std::cos(eccentricAnomaly));
}

} // anonymous namespace

Orbit::Orbit(const StateVector& stateVector, double barymass, double leptomass)
  : m_standardGravitationalParameter{standardGravitationalParameter_f(barymass, leptomass)}
{
  m_specificOrbitalEnergy = energy_f(stateVector, m_standardGravitationalParameter);
  m_specificAngularMomentum = specificAngularMomentum_f(stateVector);
  CartesianVector eccVector{eccentricityVector_f(stateVector, m_specificAngularMomentum, m_standardGravitationalParameter)};
//  m_elements.eccentricity = eccentricity(m_specificOrbitalEnergy, stdGravParam, m_specificAngularMomentum);
  m_elements.eccentricity = eccVector.norm();
  m_elements.semiMajorAxis = semiMajorAxis_f(m_standardGravitationalParameter, m_specificOrbitalEnergy);
  m_period = period_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_sweep = sweep_f(m_period);
  m_elements.inclination = inclination_f(m_specificAngularMomentum);
  CartesianVector ascVector{(0 == m_elements.inclination)
                            ? CartesianVector{}
                            : vectorOfAscendingNode_f(m_specificAngularMomentum)};
  m_elements.longitudeOfAscendingNode = longitudeOfAscendingNode_f(ascVector);
  m_elements.argumentOfPeriapsis = argumentOfPeriapsis_f(ascVector, eccVector);
  const double eccAnomaly{eccentricAnomaly_f(stateVector.position.norm(), m_elements.semiMajorAxis, m_elements.eccentricity)};
  m_elements.meanAnomalyAtEpoch = meanAnomaly_f(eccAnomaly, m_elements.eccentricity);
}

Orbit::Orbit(const OrbitalElements& elements, double barymass, double leptomass)
  : m_elements{elements}
  , m_standardGravitationalParameter{standardGravitationalParameter_f(barymass, leptomass)}
{
  m_specificOrbitalEnergy = energy_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_period = period_f(m_elements.semiMajorAxis, m_standardGravitationalParameter);
  m_sweep = sweep_f(m_period);
}

Orbit::~Orbit() = default;

} // namespace orbital