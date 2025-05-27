
#include "Locator.hh"
#include "NullLogger.hh"

namespace core::log {

NullLogger NULL_LOGGER{};
ILogger *Locator::m_logger{nullptr};

void Locator::initialize()
{
  m_logger = &NULL_LOGGER;
}

auto Locator::getLogger() -> ILogger &
{
  return *m_logger;
}

void Locator::provide(ILogger *logger)
{
  if (nullptr == logger)
  {
    m_logger = &NULL_LOGGER;
  }
  else
  {
    m_logger = logger;
  }
}

} // namespace core::log
