//
// Created by alexs on 6/24/18.
//

#include "CelestialSystem.h"
#include "OrbitalHistory.h"

#include "Body.h"
#include "CelestialBody.h"
#include "Orbit.h"

#include <Orbital.pb.h>
#include <Engine.pb.h>

using orbital::Body;
using orbital::CelestialBody;

namespace engine {

namespace {

using System_proto = Davidian::engine::System;
using BodiesMap = std::unordered_map<std::string, std::unique_ptr<orbital::Body>>;

::orbital::OrbitalElements fromProto(const Davidian::orbital::OrbitalElements& proto){
  return ::orbital::OrbitalElements{
    proto.semimajor_axis(),
    proto.eccentricity(),
    proto.inclination(),
    proto.longitude_asc_node(),
    proto.arg_periapsis(),
    proto.mean_anomaly_0()
  };
}

Davidian::orbital::OrbitalElements toProto(const ::orbital::OrbitalElements& elements){
  Davidian::orbital::OrbitalElements proto;
  proto.set_semimajor_axis(elements.semiMajorAxis);
  proto.set_eccentricity(elements.eccentricity);
  proto.set_inclination(elements.inclination);
  proto.set_longitude_asc_node(elements.longitudeOfAscendingNode);
  proto.set_arg_periapsis(elements.argumentOfPeriapsis);
  proto.set_mean_anomaly_0(elements.meanAnomalyAtEpoch);
  return proto;
}

struct CelestialSystemConstructionHelper{
  CelestialSystemConstructionHelper(const System_proto& proto, BodiesMap& bodiesRef)
  : systemProto{proto}, m_bodiesRef{bodiesRef}{}

  std::optional<int> indexOfRootBody(const System_proto& systemProto){
    for(auto i=0; i < systemProto.body_size(); ++i){
      if(systemProto.body(i).has_root_body()){return i;}
    }
    return std::optional<int>();
  }

  std::optional<std::string> emplaceRootBody(){
    auto index = indexOfRootBody(systemProto);
    if(!index.has_value()){return {};}
    const auto& proto = systemProto.body(index.value());
    m_bodiesRef.emplace(proto.name(), std::make_unique<CelestialBody>(proto.mass()));
    return proto.name();
  }

  std::vector<int> childrenOfBody(const std::string& parentName) const {
    std::vector<int> childIndices;
    for(auto i = 0; i < systemProto.body_size(); ++i){
      const auto& body = systemProto.body(i);
      if((body.has_free_body() && body.free_body().parent_body_name() == parentName)
          || (body.has_celestial_body() && body.celestial_body().parent_body_name() == parentName)){
        childIndices.push_back(i);
      }
    }
    return childIndices;
  }

  void constructCelestialBody(const Davidian::orbital::Body& body, const CelestialBody* parent){
    const auto& orbit = body.celestial_body().orbit();
    auto managedCelestialBody = std::make_unique<CelestialBody>(body.mass(), fromProto(orbit), parent);
    auto* newCB = managedCelestialBody.get();
    m_bodiesRef.emplace(body.name(), std::move(managedCelestialBody));
    auto grandchildIndices = childrenOfBody(body.name());
    emplaceChildren(grandchildIndices, newCB);
  }

  void emplaceChildren(const std::vector<int> childIndices, const CelestialBody* parent){
    for(int i : childIndices){
      const auto& body = systemProto.body(i);
      if(body.has_celestial_body()){
        constructCelestialBody(body, parent);
      } else if(body.has_free_body()){
        const auto& orbit = body.free_body().orbit();
        m_bodiesRef.emplace(body.name(), std::make_unique<Body>(body.mass(), fromProto(orbit), parent));
      }
    }
  }

  const System_proto& systemProto;

  BodiesMap& m_bodiesRef;
};

struct BodyProtoBuildHelper{
  explicit BodyProtoBuildHelper(const BodiesMap& bodiesRef)
  : m_bodiesRef{bodiesRef}
  {}

  std::tuple<const ::orbital::Body*, Davidian::orbital::Body*> setCommonData(System_proto& proto, const BodiesMap::value_type& bodyPair) const
  {
    const auto* body = bodyPair.second.get();
    auto* bodyProto = proto.add_body();
    bodyProto->set_name(bodyPair.first);
    bodyProto->set_mass(body->mass());
    return {body, bodyProto};
  }

  Davidian::orbital::BodyData* setBodyType(const ::orbital::Body* body, Davidian::orbital::Body* bodyProto) const{
    const auto* celestialBody = dynamic_cast<const CelestialBody*>(body);
    Davidian::orbital::BodyData* nonRootBodyData{nullptr};
    if(nullptr != celestialBody){
      if(nullptr == celestialBody->orbit()){
        bodyProto->mutable_root_body();
      } else {
        nonRootBodyData = bodyProto->mutable_celestial_body();
      }
    } else {
      nonRootBodyData = bodyProto->mutable_free_body();
    }
    return nonRootBodyData;
  }

  std::string parentName(const ::orbital::Body* body) const{
    const auto* parent = body->parent();
    auto parentBodyIt = std::find_if(m_bodiesRef.cbegin(), m_bodiesRef.cend(),
                                     [parent](const auto& bodyPair){ return bodyPair.second.get() == parent; });
    if(parentBodyIt != m_bodiesRef.cend()){
      return parentBodyIt->first;
    }
    return {};
  }

  void setNonRootBodyData(Davidian::orbital::BodyData* data, const ::orbital::Body* body) const{
    data->set_parent_body_name(parentName(body));
    const auto* orbit = body->orbit();
    data->mutable_orbit()->CopyFrom(toProto(orbit->orbitalElements()));
  }

  System_proto constructSystemProto() const{
    System_proto proto;
    for(const auto& bodyPair : m_bodiesRef){
      auto [body, bodyProto] = setCommonData(proto, bodyPair);
      auto* nonRootBodyData = setBodyType(body, bodyProto);
      if(nullptr != nonRootBodyData){
        setNonRootBodyData(nonRootBodyData, body);
      }
    }
    return proto;
  }

  const BodiesMap& m_bodiesRef;
};

} // anonymous namespace

CelestialSystem::CelestialSystem(const System_proto& systemProto) {
  CelestialSystemConstructionHelper helper{systemProto, m_bodies};
  auto rootName = helper.emplaceRootBody();
  if(rootName.has_value()){
    const std::unique_ptr<orbital::Body>& managedRoot = m_bodies.at(rootName.value());
    const auto* rootBody = dynamic_cast<const CelestialBody*>(managedRoot.get());
    helper.emplaceChildren(helper.childrenOfBody(rootName.value()), rootBody);
  }
}

const orbital::Body* CelestialSystem::body(const std::string& bodyName) const{
  if(0 == m_bodies.count(bodyName)){return nullptr;}
  return m_bodies.at(bodyName).get();
}

orbital::Body* CelestialSystem::body(const std::string& bodyName) {
  if(0 == m_bodies.count(bodyName)){return nullptr;}
  return m_bodies.at(bodyName).get();
}

bool CelestialSystem::isValidSystem() const {
  return !m_bodies.empty();
}

Davidian::engine::System CelestialSystem::constructCurrentSystem() const {
  return BodyProtoBuildHelper{m_bodies}.constructSystemProto();
}

OrbitalHistoryMap CelestialSystem::calculateHistories() const {
  OrbitalHistoryMap historyMap;
  for(const auto& bodyPair : m_bodies){
    if(nullptr != bodyPair.second->orbit()){
      historyMap.emplace(bodyPair.first, std::make_unique<OrbitalHistory>(*(bodyPair.second->orbit())));
    }
  }
  return historyMap;
}

CelestialSystem::~CelestialSystem() = default;

} // namespace engine
