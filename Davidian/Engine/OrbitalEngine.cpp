//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"
#include "CelestialSystem.h"
#include "CelestialSystemFileManager.h"
#include "OrbitalHistory.h"

#include "Body.h"
#include "Orbit.h"

#include "Engine.pb.h"
#include "Orbital.pb.h"

using ProtoSystem = Davidian::engine::System;

namespace engine{

namespace {

OrbitState_proto toProto(const orbital::StateVector& state, const double time){
  OrbitState_proto proto;
  proto.set_time(time);
  proto.mutable_state_vector()->mutable_position()->set_x(state.position.x());
  proto.mutable_state_vector()->mutable_position()->set_y(state.position.y());
  proto.mutable_state_vector()->mutable_position()->set_z(state.position.z());
  proto.mutable_state_vector()->mutable_velocity()->set_x(state.velocity.x());
  proto.mutable_state_vector()->mutable_velocity()->set_y(state.velocity.y());
  proto.mutable_state_vector()->mutable_velocity()->set_z(state.velocity.z());
  return proto;
}

} // anonymous namespace

OrbitalEngine::OrbitalEngine()
  : OrbitalEngine{std::make_unique<CelestialSystemFileManager>()}
{}

OrbitalEngine::OrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager)
  : m_fileManager{std::move(fileManager)}
{}

void OrbitalEngine::loadSystem(const std::string& filename) {
  useSystem(m_fileManager->loadSystem(filename));
}

System_proto OrbitalEngine::getCurrentSystem() const {
  return (nullptr == m_celestialSystem) ? System_proto{} : m_celestialSystem->constructCurrentSystem();
}

void OrbitalEngine::useSystem(const System_proto& system) {
  m_celestialSystem = std::make_unique<CelestialSystem>(system);
  m_histories = m_celestialSystem->calculateHistories();
}

bool OrbitalEngine::hasValidSystem() const {
  return (nullptr == m_celestialSystem) ? false : m_celestialSystem->isValidSystem();
}

std::optional<OrbitState_proto> OrbitalEngine::bodyStateAtTime(const std::string& bodyName,
                                                               const double seconds) const {
  if(m_histories.empty() || 0 == m_histories.count(bodyName)){return {};}

  auto bodyState = m_histories.at(bodyName)->approximateStateAtTime(seconds);

  return toProto(bodyState, seconds);
}

OrbitalEngine::~OrbitalEngine() = default;

} // namespace engine

