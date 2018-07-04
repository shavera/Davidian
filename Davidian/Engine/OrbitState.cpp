//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitState.h"
#include "NewtonSolver.h"
#include "RotationMatrices.h"

using orbital::Orbit;
using orbital::OrbitalElements;
using orbital::StateVector;
using orbital::CartesianVector;

namespace engine {

namespace{

double meanAnomaly_f(double meanAnomalyAtEpoch, double sweep, double time){
  return meanAnomalyAtEpoch + sweep*time;
}

double trueAnomaly_f(double eccentricAnomaly, double eccentricity){
  return 2*std::atan2(std::sqrt(1+eccentricity)*std::sin(eccentricAnomaly/2),
                      std::sqrt(1-eccentricity)*std::cos(eccentricAnomaly/2));
}

double radius_f(double semiMajorAxis, double eccentricity, double eccentricAnomaly){
  return semiMajorAxis*(1-eccentricity*std::cos(eccentricAnomaly));
}

StateVector orbitCoordinateStateVector(const Orbit& orbit, double radius, double trueAnomaly, double eccentricAnomaly){
  CartesianVector position{radius*std::cos(trueAnomaly), radius*std::sin(trueAnomaly), 0};
  double velocityFactor{std::sqrt(orbit.standardGravitationalParameter()*orbit.orbitalElements().semiMajorAxis)/radius};
  CartesianVector velocity{ -std::sin(eccentricAnomaly),
                            std::sqrt(1-std::pow(orbit.orbitalElements().eccentricity,2))*std::cos(eccentricAnomaly),
                            0 };
  return StateVector{position, velocityFactor*velocity};
}

StateVector globalCoordinateStateVector(const StateVector& orbitCoordinateVector, const Eigen::Matrix3d& orbitToGlobalTxfm){
  StateVector stateVector{orbitToGlobalTxfm * orbitCoordinateVector};
  return stateVector;
}

} // anonymous namespace

const OrbitState calculateOrbitState(const orbital::Orbit &orbit, double time)
{
    OrbitState state;
    state.time = time;
    const auto& elements = orbit.orbitalElements();
    const double meanAnom{meanAnomaly_f(elements.meanAnomalyAtEpoch, orbit.sweep(), state.time)};
    const double eccAnom{orbital::findEccentricAnomaly(meanAnom, elements.eccentricity)};
    const double trueAnomaly{trueAnomaly_f(eccAnom, elements.eccentricity)};
    const double radius{radius_f(elements.semiMajorAxis,  elements.eccentricity, eccAnom)};
    const StateVector orbitCoordState = orbitCoordinateStateVector(orbit, radius, trueAnomaly, eccAnom);
    const Eigen::Matrix3d rotationMatrix{orbital::OrbitToGlobal(orbit)};
    const StateVector globalVector{globalCoordinateStateVector(orbitCoordState, rotationMatrix)};
    state.stateVector = globalVector;
    return state;
}

} // namespace engine