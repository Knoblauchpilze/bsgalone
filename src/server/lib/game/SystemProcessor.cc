
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "MessageConsumerSetup.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const Uuid systemDbId)
  : core::CoreObject("processor")
  , m_systemDbId(systemDbId)
  , m_inputMessagesQueue(std::make_unique<SynchronizedMessageQueue>(
      "synchronized-message-queue-for-" + std::to_string(m_systemDbId)))
{
  setService("system");
  addModule(str(systemDbId));

  initialize();
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
  Repositories repositories{};

  SystemsConfig config{.internalMessageQueue = internalMessageQueue,
                       .outputMessageQueue   = outputMessageQueue};
  m_coordinator = std::make_shared<Coordinator>(std::move(config));

  DataSource dataSource(m_systemDbId);
  dataSource.initialize(*m_coordinator, m_entityMapper);

  m_services = createServices(ProcessingMode::SERVER, repositories, m_coordinator, m_entityMapper);
  createMessageConsumers(m_inputMessagesQueue.get(), outputMessageQueue, m_services);
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

void SystemProcessor::initialize()
{
  const chrono::TimeStep timeStep(1, chrono::Duration(chrono::Unit::MILLISECONDS, 100));
  m_timeManager = std::make_unique<chrono::TimeManager>(chrono::Tick(), timeStep);
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

    const auto elapsed = chrono::Duration{
      .unit    = chrono::Unit::MILLISECONDS,
      .elapsed = core::diffInMs(lastFrameTimestamp, thisFrameTimestamp),
    };

    const auto tick = m_timeManager->tick(elapsed);

    const auto elapsedSecond = elapsed.convert(chrono::Unit::SECONDS).elapsed;
    m_coordinator->update(elapsedSecond);

    m_processes.update(*m_coordinator, elapsedSecond);
    m_inputMessagesQueue->processMessages();

    lastFrameTimestamp = thisFrameTimestamp;

    running = m_running.load();
  }

  debug("Stopped processing for system");
}

} // namespace bsgo
