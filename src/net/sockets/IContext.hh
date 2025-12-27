
#pragma once

namespace net {

class IContext
{
  public:
  virtual ~IContext() = default;

  /// @brief - Starts the processing of network connections. This method could
  /// either be blocking or asynchronous.
  virtual void start() = 0;

  /// @brief - Requests to stop the processing of network connections. This is
  /// intended to be blocking until all network connections have been correctly
  /// terminated.
  virtual void stop() = 0;
};

} // namespace net
