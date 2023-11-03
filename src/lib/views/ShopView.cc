
#include "ShopView.hh"

namespace bsgo {

ShopView::ShopView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("shop", coordinator, repositories)
{}

auto ShopView::getShopItems() const -> std::vector<ShopItem>
{
  std::vector<ShopItem> out;
  getWeaponItems(out);
  getComputerItems(out);
  return out;
}

void ShopView::getWeaponItems(std::vector<ShopItem> &items) const
{
  const auto uuids = m_repositories.weaponRepository->findAll();
  for (const auto &weaponId : uuids)
  {
    ShopItem item{};
    item.weapon = m_repositories.weaponRepository->findOneById(weaponId);

    const auto price = m_repositories.weaponPriceRepository->findAllByWeapon(weaponId);
    for (const auto &[resourceId, cost] : price)
    {
      const auto resource = m_repositories.resourceRepository->findOneById(resourceId);
      item.price.push_back(ResourceCost{.resource = resource, .amount = cost});
    }

    items.push_back(item);
  }
}

void ShopView::getComputerItems(std::vector<ShopItem> &items) const
{
  const auto uuids = m_repositories.computerRepository->findAll();
  for (const auto &computerId : uuids)
  {
    ShopItem item{};
    item.computer = m_repositories.computerRepository->findOneById(computerId);

    const auto price = m_repositories.computerPriceRepository->findAllByComputer(computerId);
    for (const auto &[resourceId, cost] : price)
    {
      const auto resource = m_repositories.resourceRepository->findOneById(resourceId);
      item.price.push_back(ResourceCost{.resource = resource, .amount = cost});
    }

    items.push_back(item);
  }
}

} // namespace bsgo
