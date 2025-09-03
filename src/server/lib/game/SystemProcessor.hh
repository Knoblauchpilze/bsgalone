
#pragma once

#include "Coordinator.hh"
#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"
#include "Processes.hh"
#include "Services.hh"
#include "SynchronizedMessageQueue.hh"
#include "Uuid.hh"
#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>

namespace bsgo {

class SystemProcessor : public core::CoreObject
{
  public:
  SystemProcessor(const Uuid systemDbId);
  ~SystemProcessor() override;

  auto getSystemDbId() const -> Uuid;
  void pushMessage(IMessagePtr message);

  void connectToQueues(IMessageQueue *const internalMessageQueue,
                       IMessageQueue *const outputMessageQueue);
  void start();
  void stop();

  private:
  Uuid m_systemDbId{};
  IMessageQueuePtr m_inputMessagesQueue{};
  DatabaseEntityMapper m_entityMapper{};
  CoordinatorShPtr m_coordinator{};
  Services m_services{};
  Processes m_processes{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  void asyncSystemProcessing();
};

using SystemProcessorShPtr = std::shared_ptr<SystemProcessor>;
using SystemProcessorMap   = std::unordered_map<Uuid, SystemProcessorShPtr>;

} // namespace bsgo
