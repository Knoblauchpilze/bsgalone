
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "DockMessageConsumer.hh"
#include "EntityAddedMessageConsumer.hh"
#include "EntityDeletedMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "JumpCancelledMessageConsumer.hh"
#include "JumpRequestedMessageConsumer.hh"
#include "LoadingMessagesConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "ShipSelectedMessageConsumer.hh"
#include "SlotMessageConsumer.hh"
#include "TargetPickedMessageConsumer.hh"
#include "TimeUtils.hh"
#include "VelocityMessageConsumer.hh"

namespace bsgalone::server {

SystemProcessor::SystemProcessor(const core::Uuid systemDbId, core::IMessageQueueShPtr inputQueue)
  : ::core::CoreObject("processor")
  , m_systemDbId(systemDbId)
  , m_inputMessagesQueue(std::move(inputQueue))
{
  setService("system");
  addModule(core::str(systemDbId));

  initialize(systemDbId);
}

SystemProcessor::~SystemProcessor()
{
  stop();
}

auto SystemProcessor::getSystemDbId() const -> core::Uuid
{
  return m_systemDbId;
}

void SystemProcessor::connectToQueues(core::IMessageQueue *const internalMessageQueue,
                                      core::IMessageQueue *const outputMessageQueue)
{
  core::Repositories repositories{};

  core::SystemsConfig config{.internalMessageQueue = internalMessageQueue,
                             .outputMessageQueue   = outputMessageQueue};
  m_coordinator = std::make_shared<core::Coordinator>(std::move(config));

  core::DataSource dataSource(m_systemDbId);
  dataSource.initialize(*m_coordinator, m_entityMapper);

  m_services = createServices(repositories, m_coordinator, m_entityMapper);
  createMessageConsumers(outputMessageQueue);
}

void SystemProcessor::start()
{
  if (nullptr == m_coordinator)
  {
    error("Failed to start system processor", "Not yet connected to a message queue");
  }

  m_running.store(true);
  m_processingThread = std::thread(&SystemProcessor::asyncSystemProcessing, this);
}

void SystemProcessor::stop()
{
  if (!m_running.load())
  {
    return;
  }

  m_running.store(false);
  if (m_processingThread.joinable())
  {
    m_processingThread.join();
  }
}

void SystemProcessor::initialize(const core::Uuid systemDbId)
{
  core::Repositories repositories{};
  const auto data = repositories.systemRepository->findOneById(m_systemDbId);
  m_timeManager   = std::make_unique<chrono::TimeManager>(data.currentTick, data.step);
  info("System will start at tick " + data.currentTick.str());

  m_processes = std::make_unique<Processes>(systemDbId, m_inputMessagesQueue.get());
}

void SystemProcessor::asyncSystemProcessing()
{
  bool running{true};
  auto lastFrameTimestamp = ::core::now();

  debug("Started processing for system");
  while (running)
  {
    constexpr auto SLEEP_DURATION_WHEN_PROCESSING = ::core::Milliseconds(50);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_PROCESSING);

    const auto thisFrameTimestamp = ::core::now();

    const auto elapsed = chrono::Duration{
      .unit    = chrono::Unit::MILLISECONDS,
      .elapsed = ::core::diffInMs(lastFrameTimestamp, thisFrameTimestamp),
    };

    const auto data = m_timeManager->tick(elapsed);
    m_coordinator->update(data);
    m_processes->update(*m_coordinator, data);
    m_inputMessagesQueue->processEvents();

    lastFrameTimestamp = thisFrameTimestamp;

    running = m_running.load();
  }

  debug("Stopped processing for system");
}

void SystemProcessor::createMessageConsumers(core::IMessageQueue *const outputMessagesQueue)
{
  m_inputMessagesQueue->addListener(
    std::make_unique<ShipSelectedMessageConsumer>(m_services,
                                                  m_inputMessagesQueue.get(),
                                                  outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<PurchaseMessageConsumer>(m_services,
                                              m_inputMessagesQueue.get(),
                                              outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<EquipMessageConsumer>(m_services,
                                           m_inputMessagesQueue.get(),
                                           outputMessagesQueue));

  m_inputMessagesQueue->addListener(std::make_unique<DockMessageConsumer>(m_services,
                                                                          m_inputMessagesQueue.get(),
                                                                          outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<SlotMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<VelocityMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<TargetPickedMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<JumpCancelledMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<JumpRequestedMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<EntityDeletedMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<EntityAddedMessageConsumer>(m_services, outputMessagesQueue));

  m_inputMessagesQueue->addListener(
    std::make_unique<LoadingMessagesConsumer>(m_services, outputMessagesQueue));
}

} // namespace bsgalone::server
