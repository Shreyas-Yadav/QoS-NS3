// File: filter-element-protocol.h

#ifndef FILTER_ELEMENT_PROTOCOL_H
#define FILTER_ELEMENT_PROTOCOL_H

#include "filter-element.h"
#include <cstdint>

namespace ns3 {

/**
 * \brief Matches packets whose IPv4 protocol number equals the given value.
 */
class FilterElementProtocol : public FilterElement
{
public:
  explicit FilterElementProtocol(uint8_t protocol);

  // Returns true if the packet's IP protocol field == m_protocol
  virtual bool match(Ptr<Packet> p) const override;

private:
  uint8_t m_protocol;
};

} // namespace ns3

#endif // FILTER_ELEMENT_PROTOCOL_H
