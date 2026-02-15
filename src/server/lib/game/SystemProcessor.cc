
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "MessageConsumerSetup.hh"
#include "TimeUtils.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const Uuid systemDbId,
                                 bsgalone::core::IMessageQueueShPtr inputQueue)
  : core::CoreObject("processor")
  , m_systemDbId(systemDbId)
  , m_inputMessagesQueue(std::move(inputQueue))
{
  setService("system");
  addModule(str(systemDbId));

  initialize(systemDbId);
}

SystemProcessor::~SystemProcessor()
{
  stop();
}

auto SystemProcessor::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

void SystemProcessor::connectToQueues(bsgalone::core::IMessageQueue *const internalMessageQueue,
                                      bsgalone::core::IMessageQueue *const outputMessageQueue)
{
  Repositories repositories{};

  SystemsConfig config{.internalMessageQueue = internalMessageQueue,
                       .outputMessageQueue   = outputMessageQueue};
  m_coordinator = std::make_shared<Coordinator>(std::move(config));

  DataSource dataSource(m_systemDbId);
  dataSource.initialize(*m_coordinator, m_entityMapper);

  m_services = createServices(repositories, m_coordinator, m_entityMapper);
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

void SystemProcessor::initialize(const Uuid systemDbId)
{
  Repositories repositories{};
  const auto tickConfig = repositories.tickRepository->findOneBySystem(m_systemDbId);
  m_timeManager = std::make_unique<chrono::TimeManager>(tickConfig.currentTick, tickConfig.step);
  info("System will start at tick " + tickConfig.currentTick.str());

  m_processes = std::make_unique<Processes>(systemDbId, m_inputMessagesQueue.get());
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

    const auto data = m_timeManager->tick(elapsed);
    m_coordinator->update(data);
    m_processes->update(*m_coordinator, data);
    m_inputMessagesQueue->processMessages();

    lastFrameTimestamp = thisFrameTimestamp;

    running = m_running.load();
  }

  debug("Stopped processing for system");
}

} // namespace bsgo
