
#pragma once

#include "AbstractView.hh"
#include "ResourceData.hh"
#include "Uuid.hh"
#include <memory>
#include <unordered_map>

namespace pge {

class ResourceView : public AbstractView
{
  public:
  ResourceView();
  ~ResourceView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getResourceName(const bsgo::Uuid resource) const -> std::string;

  protected:
  void handleMessageInternal(const bsgalone::core::IMessage &message) override;

  private:
  std::unordered_map<bsgo::Uuid, bsgo::ResourceData> m_resources{};
};

using ResourceViewShPtr = std::shared_ptr<ResourceView>;

} // namespace pge
