#include "OrbitalHistory.h"

namespace engine {

using orbital::StateVector;

namespace {
StateVector interpolate(double t0, const StateVector& s0, double t1, const StateVector& s1, double t){
  return {interpolate(t0, s0.position, t1, s1.position, t), interpolate(t0, s0.velocity, t1, s1.velocity, t)};
}
} // anonymous namespace

OrbitalHistory::OrbitalHistory(const orbital::Orbit& orbit)
  : m_orbit{orbit}
{
  const double deltaT{m_orbit.period()/m_history.size()};
  double time = 0;
  std::generate(m_history.begin(), m_history.end(), [orbit, deltaT, &time](){
    //double time = deltaT * m++;
    auto state = calculateOrbitState(orbit, time);
    time += deltaT;
    return state;
  });
}

orbital::StateVector OrbitalHistory::approximateStateAtTime(double seconds) const {
  double adjustedTime = std::fmod(seconds, m_orbit.period());
  if(adjustedTime < 0) {adjustedTime += m_orbit.period();}

  auto bound = std::upper_bound(m_history.cbegin(), m_history.cend(), adjustedTime,
                                [](const double& time, const OrbitState& state){ return time < state.time; });

  const OrbitState *v0{nullptr}, *v1{nullptr};
  if(bound == m_history.cend()){
    v1 = &(m_history.front());
    v0 = &(m_history.back());
  } else {
    v1 = &(*bound);
    v0 = &(*std::prev(bound));
  }

  return interpolate(v0->time, v0->stateVector, v1->time, v1->stateVector, adjustedTime);
}

} // namespace engine