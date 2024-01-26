
#pragma once

#include "Coordinator.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

struct SystemProcessingConfig
{
  Uuid systemDbId{};
  IMessageQueue *const inputMessageQueue{};
  IMessageQueue *const outputMessageQueue{};
};

class SystemProcessor : public utils::CoreObject
{
  public:
  SystemProcessor(const SystemProcessingConfig &config);
  ~SystemProcessor() override = default;

  private:
  CoordinatorShPtr m_coordinator{};
  Services m_services{};

  void initialize(const SystemProcessingConfig &config);
};

using SystemProcessorPtr = std::unique_ptr<SystemProcessor>;

} // namespace bsgo
