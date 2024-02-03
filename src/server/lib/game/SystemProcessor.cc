
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "MessageConsumerSetup.hh"
#include "NetworkSystem.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const Uuid systemDbId)
  : utils::CoreObject("processor")
  , m_systemDbId(systemDbId)
{
  setService("system");
  addModule(str(systemDbId));
}

SystemProcessor::~SystemProcessor()
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

auto SystemProcessor::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

void SystemProcessor::pushMessage(IMessagePtr message)
{
  m_inputMessagesQueue->pushMessage(std::move(message));
}

void SystemProcessor::connectToQueue(IMessageQueue *const outputMessageQueue)
{
  DataSource dataSource{DataLoadingMode::SERVER};
  const auto repositories = dataSource.repositories();

  auto networkSystem = std::make_unique<NetworkSystem>(repositories);
  m_coordinator      = std::make_shared<Coordinator>(std::move(networkSystem), outputMessageQueue);

  dataSource.setSystemDbId(m_systemDbId);
  dataSource.initialize(*m_coordinator, m_entityMapper);

  m_services = createServices(repositories, m_coordinator, m_entityMapper);
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

void SystemProcessor::asyncSystemProcessing()
{
  bool running{true};
  auto lastFrameTimestamp = utils::now();

  debug("Started processing for system");
  while (running)
  {
    constexpr auto SLEEP_DURATION_WHEN_PROCESSING = utils::Milliseconds(50);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_PROCESSING);

    const auto thisFrameTimestamp = utils::now();
    const auto elapsedMs          = utils::diffInMs(lastFrameTimestamp, thisFrameTimestamp);

    constexpr auto MS_IN_A_SECOND = 1'000;
    m_coordinator->update(elapsedMs / MS_IN_A_SECOND);
    m_inputMessagesQueue->processMessages();

    lastFrameTimestamp = thisFrameTimestamp;

    running = m_running.load();
  }

  debug("Stopped processing for system");
}

} // namespace bsgo
