
#pragma once

#include "IMessageQueue.hh"
#include "ISystem.hh"
#include "NetworkSystem.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <unordered_set>
#include <vector>

namespace bsgo {

struct SystemsConfig
{
  ISystemPtr networkSystem{};

  IMessageQueue *const internalMessageQueue{};
  IMessageQueue *const outputMessageQueue{};

  std::unordered_set<SystemType> ignoredSystems{};
};

class Systems : public utils::CoreObject
{
  public:
  Systems(SystemsConfig &&config);
  ~Systems() override = default;

  void update(Coordinator &coordinator, const float elapsedSeconds) const;

  private:
  std::vector<ISystemPtr> m_systems{};

  void initialize(SystemsConfig &&config);
};

using SystemsPtr = std::unique_ptr<Systems>;

} // namespace bsgo
