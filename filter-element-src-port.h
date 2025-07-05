// File: filter-element-src-port.h

#ifndef FILTER_ELEMENT_SRC_PORT_H
#define FILTER_ELEMENT_SRC_PORT_H

#include "filter-element.h"
#include <cstdint>

namespace ns3 {

/**
 * \brief Matches packets whose TCP/UDP source port equals the given value.
 */
class FilterElementSrcPort : public FilterElement
{
public:
  FilterElementSrcPort(uint16_t port);

  // Returns true if the packet's transport source port == m_port
  virtual bool match(Ptr<Packet> p) const override;

private:
  uint16_t m_port;
};

} // namespace ns3

#endif // FILTER_ELEMENT_SRC_PORT_H
