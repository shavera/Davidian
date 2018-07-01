//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITALENGINE_H
#define DAVIDIAN_ORBITALENGINE_H

#include "EngineInterface.h"

#include <memory>
#include <unordered_map>

namespace engine {

class CelestialSystemFileManager;
class CelestialSystem;
class OrbitalHistory;

class OrbitalEngine : public EngineInterface{
public:
  OrbitalEngine();
  virtual ~OrbitalEngine();

  void loadSystem(const std::string& filename) override;
  void useSystem(const System_proto& system) override;

  bool hasValidSystem() const override;

  System_proto getCurrentSystem() override;

  std::optional<OrbitState_proto> bodyStateAtTime(const std::string& bodyName, const double seconds) const override;

protected:
  explicit OrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager);

private:
  std::unique_ptr<CelestialSystemFileManager> m_fileManager;
  std::unique_ptr<CelestialSystem> m_celestialSystem;
  std::unordered_map<std::string, std::unique_ptr<OrbitalHistory>> m_histories;
};

} // namespace engine

#endif //DAVIDIAN_ORBITALENGINE_H
