
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "MessageConsumerSetup.hh"
#include "NetworkSystem.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const Uuid systemDbId)
  : core::CoreObject("processor")
  , m_systemDbId(systemDbId)
  , m_inputMessagesQueue(std::make_unique<SynchronizedMessageQueue>(
      "synchronized-message-queue-for-" + std::to_string(m_systemDbId)))
{
  setService("system");
  addModule(str(systemDbId));
}

SystemProcessor::~SystemProcessor()
{
  stop();
}

auto SystemProcessor::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

void SystemProcessor::pushMessage(IMessagePtr message)
{
  m_inputMessagesQueue->pushMessage(std::move(message));
}

void SystemProcessor::connectToQueues(IMessageQueue *const internalMessageQueue,
                                      IMessageQueue *const outputMessageQueue)
{
  DataSource dataSource{DataLoadingMode::SERVER};
  const auto repositories = dataSource.repositories();

  SystemsConfig config{.networkSystem        = std::make_unique<NetworkSystem>(repositories),
                       .internalMessageQueue = internalMessageQueue,
                       .outputMessageQueue   = outputMessageQueue};
  m_coordinator = std::make_shared<Coordinator>(std::move(config));

  dataSource.setSystemDbId(m_systemDbId);
  dataSource.initialize(*m_coordinator, m_entityMapper);

  m_services = createServices(ProcessingMode::SERVER, repositories, m_coordinator, m_entityMapper);
  // TODO: We could add a PlayerListMessageConsumer which would react on the message
  // with that type and populate the list of players with the ones from the system
  // This service would already have access to the broadcast queue so we can then send
  // this message to the client.
  createMessageConsumers(*m_inputMessagesQueue, outputMessageQueue, m_services);
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

void SystemProcessor::asyncSystemProcessing()
{
  bool running{true};
  auto lastFrameTimestamp = core::now();

  debug("Started processing for system");
  while (running)
  {
    constexpr auto SLEEP_DURATION_WHEN_PROCESSING = core::Milliseconds(50);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_PROCESSING);

    const auto thisFrameTimestamp = core::now();
    const auto elapsedMs          = core::diffInMs(lastFrameTimestamp, thisFrameTimestamp);

    constexpr auto MS_IN_A_SECOND = 1'000;
    m_coordinator->update(elapsedMs / MS_IN_A_SECOND);
    m_inputMessagesQueue->processMessages();

    lastFrameTimestamp = thisFrameTimestamp;

    running = m_running.load();
  }

  debug("Stopped processing for system");
}

} // namespace bsgo
