
#include "ResourceView.hh"
#include "ResourceListMessage.hh"

namespace pge {

ResourceView::ResourceView()
  : AbstractView("resource", {bsgo::MessageType::RESOURCE_LIST})
{}

bool ResourceView::isReady() const noexcept
{
  return !m_resources.empty();
}

void ResourceView::reset()
{
  m_resources.clear();
}

auto ResourceView::getResourceName(const bsgo::Uuid resource) const -> std::string
{
  const auto maybeResource = m_resources.find(resource);

  if (maybeResource == m_resources.end())
  {
    error("Failed to get resource", "No resource registered for id " + bsgo::str(resource));
  }

  return maybeResource->second.name;
}

void ResourceView::handleMessageInternal(const bsgo::IMessage &message)
{
  const auto resourceList = message.as<bsgo::ResourceListMessage>();
  const auto resources    = resourceList.getResourcesData();

  m_resources.clear();

  for (const auto &resource : resources)
  {
    m_resources.insert({resource.dbId, resource});
  }
}

} // namespace pge
