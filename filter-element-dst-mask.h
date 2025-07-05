// File: filter-element-dst-mask.h

#ifndef FILTER_ELEMENT_DST_MASK_H
#define FILTER_ELEMENT_DST_MASK_H

#include "filter-element.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \brief Matches packets whose IPv4 destination address falls within a given subnet.
 *
 * Performs ((dst & mask) == (network & mask)).
 */
class FilterElementDstMask : public FilterElement
{
public:
  /**
   * \param network   The base network address (e.g., 192.168.1.0)
   * \param mask      The subnet mask      (e.g., 255.255.255.0)
   */
  FilterElementDstMask(Ipv4Address network, Ipv4Mask mask);
  

  // Returns true if (dst & mask) == (network & mask)
  virtual bool match(Ptr<Packet> p) const override;

private:
  Ipv4Address m_network_ip;
  Ipv4Mask    m_mask;
};

} // namespace ns3

#endif // FILTER_ELEMENT_DST_MASK_H
