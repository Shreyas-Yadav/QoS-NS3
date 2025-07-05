/*
 * This program implements a filter element that matches packets
 * based on their IPv4 destination address.
 */

#ifndef FILTER_ELEMENT_DST_IP_H
#define FILTER_ELEMENT_DST_IP_H

#include "filter-element.h"
#include "ns3/ipv4-header.h"

namespace ns3 {

/**
 * \brief Matches packets whose IPv4 destination address equals the given value
 */
class FilterElementDstIp : public FilterElement
{
public:
  /**
   * \brief Constructor
   * \param dst IPv4 address to match against packet destination
   */
  FilterElementDstIp (Ipv4Address dst);
  
  /**
   * \brief Check if packet's IPv4 destination matches the configured address
   * \param p The packet to check
   * \return true if the packet's IPv4 destination equals m_dst
   */
  virtual bool match (Ptr<Packet> p) const override;

private:
  Ipv4Address m_dst; //!< IPv4 address to match against
};

} // namespace ns3

#endif // FILTER_ELEMENT_DST_IP_H
