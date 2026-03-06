
#include "ResourceView.hh"
#include "ResourceListMessage.hh"

namespace bsgalone::client {

ResourceView::ResourceView()
  : AbstractView("resource", {core::MessageType::RESOURCE_LIST})
{}

bool ResourceView::isReady() const noexcept
{
  return !m_resources.empty();
}

void ResourceView::reset()
{
  m_resources.clear();
}

auto ResourceView::getResourceName(const core::Uuid resource) const -> std::string
{
  const auto maybeResource = m_resources.find(resource);

  if (maybeResource == m_resources.end())
  {
    error("Failed to get resource", "No resource registered for id " + core::str(resource));
  }

  return maybeResource->second.name;
}

void ResourceView::handleMessageInternal(const core::IMessage &message)
{
  const auto resourceList = message.as<core::ResourceListMessage>();
  const auto resources    = resourceList.getResourcesData();

  m_resources.clear();

  for (const auto &resource : resources)
  {
    m_resources.insert({resource.dbId, resource});
  }
}

} // namespace bsgalone::client
