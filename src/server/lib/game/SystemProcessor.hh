
#pragma once

#include "Coordinator.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "Uuid.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <thread>

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
  ~SystemProcessor() override;

  void start();

  private:
  CoordinatorShPtr m_coordinator{};
  Services m_services{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  void initialize(const SystemProcessingConfig &config);
  void asyncSystemProcessing();
};

using SystemProcessorPtr = std::unique_ptr<SystemProcessor>;

} // namespace bsgo
