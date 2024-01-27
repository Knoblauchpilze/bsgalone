
#include "SystemProcessor.hh"
#include "DataSource.hh"
#include "ServerMessageConsumerUtils.hh"

namespace bsgo {

SystemProcessor::SystemProcessor(const SystemProcessingConfig &config)
  : utils::CoreObject("processor")
{
  setService("system");
  addModule(str(config.systemDbId));

  initialize(config);
}

void SystemProcessor::initialize(const SystemProcessingConfig &config)
{
  DataSource dataSource{};
  const auto repositories = dataSource.repositories();

  m_coordinator = std::make_shared<Coordinator>(config.inputMessageQueue);

  m_services = createServices(repositories, m_coordinator);
  createMessageConsumers(*config.inputMessageQueue, config.outputMessageQueue, m_services);

  dataSource.setSystemDbId(config.systemDbId);
  dataSource.initialize(*m_coordinator);
}

} // namespace bsgo
