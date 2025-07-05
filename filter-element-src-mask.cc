// File: filter-element-src-mask.cc

#include "filter-element-src-mask.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"

namespace ns3 {


FilterElementSrcMask::FilterElementSrcMask(Ipv4Address network, Ipv4Mask mask)
  : m_network_ip(network), m_mask(mask)
{}



bool
FilterElementSrcMask::match(Ptr<Packet> p) const
{
  // std::cout << "FilterElementSrcMask::match - Checking if packet source IP matches network "
  //           << m_network_ip << " with mask " << m_mask << std::endl;

  // Peek at the IPv4 header without consuming the packet
  Ptr<Packet> copy = p->Copy();
  
  PppHeader pppHeader;
  if(!copy->RemoveHeader(pppHeader)){
    return false;
  }

  Ipv4Header ipv4Header;
  
  if (!copy->RemoveHeader(ipv4Header)) {
    // std::cout << "  Failed to peek IPv4 header, returning false" << std::endl;
    return false;
  }
  
  Ipv4Address src = ipv4Header.GetSource();
  return src.CombineMask(m_mask) == m_network_ip.CombineMask(m_mask);
}

}
