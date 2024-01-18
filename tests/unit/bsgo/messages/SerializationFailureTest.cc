
#include "Uuid.hh"
#include <core_utils/SerializationUtils.hh>
#include <gtest/gtest.h>
#include <sstream>

using namespace ::testing;

namespace bsgo {
namespace {
template<typename T>
bool serializeAndTruncate(const T &expected)
{
  std::ostringstream out{};
  utils::serialize(out, expected);
  const auto serialized = out.str();
  std::istringstream in(serialized.substr(0, 1));

  T output{};
  return utils::deserialize(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_Failure, Uuid)
{
  EXPECT_FALSE(serializeAndTruncate(Uuid{2}));
}

TEST(Unit_Bsgo_Serialization_Failure, String)
{
  EXPECT_FALSE(serializeAndTruncate(std::string{"some-string"}));
}

TEST(Unit_Bsgo_Serialization_Failure, OptionalUuid)
{
  EXPECT_FALSE(serializeAndTruncate(std::optional<Uuid>{Uuid{4}}));
}

} // namespace bsgo
