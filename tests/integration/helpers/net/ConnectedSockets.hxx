
#pragma once

#include "ConnectedSockets.hh"
#include "SocketUtils.hh"

namespace test {

template<typename T>
inline void ConnectedSockets::writeServer(const T &data) const
{
  const char *begin = reinterpret_cast<const char *>(&data);
  std::vector<char> rawData(begin, begin + sizeof(T));

  writeTo(*server, rawData);
}

template<>
inline void ConnectedSockets::writeServer(const std::string &data) const
{
  writeTo(*server, std::vector<char>(data.begin(), data.end()));
}

} // namespace test
