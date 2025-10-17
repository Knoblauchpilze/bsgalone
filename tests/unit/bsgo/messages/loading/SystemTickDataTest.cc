
#include "Comparison.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const SystemTickData &value, SystemTickData &output)
{
  std::ostringstream out{};
  serializeSystemTickData(out, value);
  std::istringstream in(out.str());
  return deserializeSystemTickData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_SystemTickData, EqualWhenDbIdIsEqual)
{
  SystemTickData data1{.dbId        = Uuid{1234},
                       .currentTick = chrono::Tick(1.2f),
                       .step        = chrono::TimeStep(14,
                                                chrono::Duration{.unit    = chrono::Unit::SECONDS,
                                                                        .elapsed = 7.94f})};

  SystemTickData data2{.dbId        = Uuid{1234},
                       .currentTick = chrono::Tick::fromInt(17),
                       .step        = chrono::TimeStep(121,
                                                chrono::Duration{.unit = chrono::Unit::MILLISECONDS,
                                                                        .elapsed = 98.0912f})};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_SystemTickData, DifferentWhenDbIdIsDifferent)
{
  SystemTickData data1{.dbId        = Uuid{1234},
                       .currentTick = chrono::Tick(1.2f),
                       .step        = chrono::TimeStep(14,
                                                chrono::Duration{.unit    = chrono::Unit::SECONDS,
                                                                        .elapsed = 7.94f})};

  SystemTickData data2 = data1;
  data2.dbId           = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_SystemTickData, Basic)
{
  SystemTickData input{.dbId        = Uuid{1234},
                       .currentTick = chrono::Tick(0.04705f),
                       .step        = chrono::TimeStep(14,
                                                chrono::Duration{.unit = chrono::Unit::MILLISECONDS,
                                                                        .elapsed = 7.94f})};

  SystemTickData output{.dbId        = Uuid{14},
                        .currentTick = chrono::Tick::fromInt(1014),
                        .step        = chrono::TimeStep(97,
                                                 chrono::Duration{.unit    = chrono::Unit::SECONDS,
                                                                         .elapsed = 0.1025f})};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertSystemTickDataAreEqual(output, input);
}

} // namespace bsgo
