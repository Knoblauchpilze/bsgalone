

#pragma once

#include "AbstractMessage.hh"

namespace bsgo {

class ConnectionMessage : public bsgalone::core::AbstractMessage
{
  public:
  ConnectionMessage();
  ~ConnectionMessage() override = default;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;
};

} // namespace bsgo
