//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_ORBITALENGINE_H
#define DAVIDIAN_ORBITALENGINE_H

#include "EngineInterface.h"

#include <memory>

namespace engine {

class CelestialSystemFileManager;

class OrbitalEngine : public EngineInterface{
public:
  OrbitalEngine();
  virtual ~OrbitalEngine();

  void loadSystem(const std::string& filename) override;
  void useSystem(const System_proto& system) override;

  bool hasValidSystem() const override;

  System_proto getCurrentSystem() override;

  System_proto advanceSystemToTime(double t) override;

  std::optional<OrbitState_proto> bodyStateAtTime(const std::string& bodyName, double t) const override;

protected:
  explicit OrbitalEngine(std::unique_ptr<CelestialSystemFileManager>&& fileManager);

private:
  std::unique_ptr<CelestialSystemFileManager> m_fileManager;
};

} // namespace engine

#endif //DAVIDIAN_ORBITALENGINE_H
