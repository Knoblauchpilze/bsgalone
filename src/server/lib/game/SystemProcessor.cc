
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "MessageConsumerSetup.hh"
#include "NetworkSystem.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const SystemProcessingConfig &config)
  : utils::CoreObject("processor")
  , m_systemDbId(config.systemDbId)
{
  setService("system");
  addModule(str(config.systemDbId));

  initialize(config);
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

void SystemProcessor::start()
{
  m_running.store(true);
  m_processingThread = std::thread(&SystemProcessor::asyncSystemProcessing, this);
}

void SystemProcessor::initialize(const SystemProcessingConfig &config)
{
  DataSource dataSource{DataLoadingMode::SERVER};
  const auto repositories = dataSource.repositories();

  auto networkSystem = std::make_unique<NetworkSystem>(repositories);
  m_coordinator = std::make_shared<Coordinator>(std::move(networkSystem), config.outputMessageQueue);

  m_services = createServices(repositories, m_coordinator);
  createMessageConsumers(*m_inputMessagesQueue, config.outputMessageQueue, m_services);

  dataSource.setSystemDbId(m_systemDbId);
  dataSource.initialize(*m_coordinator);
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
