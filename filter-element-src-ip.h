
#ifndef FILTER_ELEMENT_SRC_IP_H
#define FILTER_ELEMENT_SRC_IP_H

#include "filter-element.h"
#include "ns3/ipv4-header.h"

namespace ns3 {

/**
 * \brief Matches packets whose IPv4 source address equals the given value.
 */
class FilterElementSrcIp : public FilterElement {
public:
  /**
   * \brief Constructor
   * \param src The source IP address to match against
   */
  FilterElementSrcIp(Ipv4Address src);
  
 
  /**
   * \brief Check if a packet's source IP matches the configured value
   * \param p The packet to check
   * \return true if the packet's IPv4 source matches m_src, false otherwise
   */
  virtual bool match(Ptr<Packet> p) const override;

private:
  Ipv4Address m_src;  ///< The source IP address to match against
};

} // namespace ns3

#endif // FILTER_ELEMENT_SRC_IP_H
