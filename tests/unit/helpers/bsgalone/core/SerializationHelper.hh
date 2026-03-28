
#pragma once

#include "IMessage.hh"

namespace test {

template<typename T>
auto serializeAndDeserializeMessage(const T &value) -> bsgalone::core::IMessagePtr;

} // namespace test

#include "SerializationHelper.hxx"
