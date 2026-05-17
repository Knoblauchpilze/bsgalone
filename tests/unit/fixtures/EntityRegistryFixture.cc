
#include "EntityRegistryFixture.hh"

namespace test {

EntityRegistryFixture::EntityRegistryFixture()
  : m_entityRegistry(std::make_shared<bsgalone::core::EntityRegistry>())
{}

void EntityRegistryFixture::SetUp() {}

void EntityRegistryFixture::TearDown() {}

auto EntityRegistryFixture::entityRegistry() const -> bsgalone::core::EntityRegistryShPtr
{
  return m_entityRegistry;
}

} // namespace test
