
#pragma once

#include "CoreObject.hh"
#include "IEcsCoordinator.hh"
#include "TimeManager.hh"
#include "Uuid.hh"
#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>

namespace bsgalone::core {

class SystemProcessor : public ::core::CoreObject
{
  public:
  SystemProcessor(const std::string &name,
                  IEcsCoordinatorPtr coordinator,
                  chrono::ITimeManagerPtr timeManager);
  ~SystemProcessor() override;

  void start();
  void stop();

  private:
  Uuid m_systemDbId{};

  std::mutex m_locker{};
  bool m_running{false};
  std::thread m_processingThread{};

  IEcsCoordinatorPtr m_coordinator{};
  chrono::ITimeManagerPtr m_timeManager{};

  void asyncProcessing();

  /// @brief - Used to check whether the asynchronous processing was started and
  /// if yes, stops it.
  /// This method aims at wrapping acquiring the `m_locker` in a dedicated block
  /// to make the join call possible on the processing thread.
  /// @return - true if the processing thread was started and needs to be joined
  bool tryStopProcessing();
};

using SystemProcessorShPtr = std::shared_ptr<SystemProcessor>;

} // namespace bsgalone::core
