
#pragma once

#include "ILogger.hh"
#include <string>

namespace core::log {

class Locator
{
  public:
  static void initialize();

  static auto getLogger() -> ILogger &;

  static void provide(ILogger *logger);

  private:
  static ILogger *m_logger;
};

} // namespace core::log
