
#pragma once

#include "IMessage.hh"

namespace test {

template<typename T>
void serializeAndDeserializeMessage(const T &value, T &output);

template<typename T>
auto serializeAndDeserializePlayerMessage(const T &value) -> bsgalone::core::IMessagePtr;

} // namespace test

#include "SerializationHelper.hxx"
