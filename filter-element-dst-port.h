/*
 * This program implements a filter element that matches packets
 * based on their TCP/UDP destination port.
 */

#ifndef FILTER_ELEMENT_DST_PORT_H
#define FILTER_ELEMENT_DST_PORT_H

#include "filter-element.h"
#include <cstdint>

namespace ns3 {

/**
 * \brief Matches packets whose TCP/UDP destination port equals the given value
 */
class FilterElementDstPort : public FilterElement
{
public:
  /**
   * \brief Constructor
   * \param port Port number to match against packet destination port
   */
  FilterElementDstPort (uint16_t port);
  
  /**
   * \brief Check if packet's destination port matches the configured port
   * \param p The packet to check
   * \return true if the packet's transport destination port equals m_port
   */
  virtual bool match (Ptr<Packet> p) const override;

private:
  uint16_t m_port; //!< Port number to match against
};

} // namespace ns3

#endif // FILTER_ELEMENT_DST_PORT_H
