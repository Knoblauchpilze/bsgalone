
#include "PortFixture.hh"

namespace test {

std::atomic_int PortFixture::NEXT_PORT{2000};

PortFixture::PortFixture()
  : m_port(NEXT_PORT.fetch_add(1))
{}

void PortFixture::SetUp() {}

void PortFixture::TearDown() {}

auto PortFixture::port() const -> int
{
  return m_port;
}

} // namespace test
