
#include "SystemProcessor.hh"
#include "TimeUtils.hh"

namespace bsgalone::core {

SystemProcessor::SystemProcessor(const std::string &name,
                                 ForRunningSimulationPtr coordinator,
                                 chrono::ITimeManagerPtr timeManager)
  : ::core::CoreObject("processor")
  , m_coordinator(std::move(coordinator))
  , m_timeManager(std::move(timeManager))
{
  setService("system");
  addModule(name);

  if (m_coordinator == nullptr)
  {
    throw std::invalid_argument("Expected non null coordinator");
  }
  if (m_timeManager == nullptr)
  {
    throw std::invalid_argument("Expected non null time manager");
  }
}

SystemProcessor::~SystemProcessor()
{
  stop();
}

void SystemProcessor::start()
{
  const std::lock_guard guard(m_locker);
  if (m_running)
  {
    warn("Processor already started");
    return;
  }

  m_running          = true;
  m_processingThread = std::thread(&SystemProcessor::asyncProcessing, this);
}

void SystemProcessor::stop()
{
  if (tryStopProcessing())
  {
    m_processingThread.join();
  }
}

namespace {
/// @brief - Defines the time allocated to process one tick of
/// the simulation. This value is expressed in milliseconds and
/// currently represent 20 update cycles per second.
constexpr ::core::Milliseconds SLEEP_DURATION_WHEN_PROCESSING{50};
} // namespace

void SystemProcessor::asyncProcessing()
{
  bool running{true};
  auto lastFrameTimestamp = ::core::now();

  debug("Started processing for environment");

  while (running)
  {
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_PROCESSING);

    const auto thisFrameTimestamp = ::core::now();

    const auto elapsed = chrono::Duration{
      .unit    = chrono::Unit::MILLISECONDS,
      .elapsed = ::core::diffInMs(lastFrameTimestamp, thisFrameTimestamp),
    };

    const auto data = m_timeManager->tick(elapsed);
    m_coordinator->update(data);

    lastFrameTimestamp = thisFrameTimestamp;

    const std::lock_guard guard(m_locker);
    running = m_running;
  }

  debug("Stopped processing for environment");
}

bool SystemProcessor::tryStopProcessing()
{
  const std::lock_guard guard(m_locker);
  if (!m_running)
  {
    return false;
  }

  m_running = false;

  return true;
}

} // namespace bsgalone::core
