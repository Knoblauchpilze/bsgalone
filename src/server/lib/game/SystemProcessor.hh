
#pragma once

#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "IMessageQueue.hh"
#include "Services.hh"
#include "SynchronizedMessageQueue.hh"
#include "Uuid.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <thread>

namespace bsgo {

class SystemProcessor : public utils::CoreObject
{
  public:
  SystemProcessor(const Uuid systemDbId);
  ~SystemProcessor() override;

  auto getSystemDbId() const -> Uuid;
  void pushMessage(IMessagePtr message);

  void onAsteroidDestroyed(const Uuid asteroidDbId);
  void onShipDestroyed(const Uuid shipDbId);

  void connectToQueues(IMessageQueue *const internalMessageQueue,
                       IMessageQueue *const outputMessageQueue);
  void start();

  private:
  Uuid m_systemDbId{};
  IMessageQueuePtr m_inputMessagesQueue{std::make_unique<SynchronizedMessageQueue>()};
  DatabaseEntityMapper m_entityMapper{};
  CoordinatorShPtr m_coordinator{};
  Services m_services{};

  std::atomic_bool m_running{false};
  std::thread m_processingThread{};

  void asyncSystemProcessing();
};

using SystemProcessorShPtr = std::shared_ptr<SystemProcessor>;

} // namespace bsgo
