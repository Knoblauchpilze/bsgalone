
#pragma once

#include "Coordinator.hh"
#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"
#include "Processes.hh"
#include "Services.hh"
#include "SynchronizedMessageQueue.hh"
#include "TimeManager.hh"
#include "Uuid.hh"
#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>

namespace bsgo {

class SystemProcessor : public core::CoreObject
{
  public:
  SystemProcessor(const Uuid systemDbId, IMessageQueueShPtr inputQueue);
  ~SystemProcessor() override;

  auto getSystemDbId() const -> Uuid;

  void connectToQueues(IMessageQueue *const internalMessageQueue,
                       IMessageQueue *const outputMessageQueue);
  void start();
  void stop();

  private:
  Uuid m_systemDbId{};
  IMessageQueueShPtr m_inputMessagesQueue{};
  DatabaseEntityMapper m_entityMapper{};

  chrono::TimeManagerPtr m_timeManager{};
  CoordinatorShPtr m_coordinator{};
  Services m_services{};
  ProcessesPtr m_processes{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  void initialize(const Uuid systemDbId);
  void asyncSystemProcessing();
};

using SystemProcessorShPtr = std::shared_ptr<SystemProcessor>;

} // namespace bsgo
