#include "filter-element-dst-mask.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"

namespace ns3 {


FilterElementDstMask::FilterElementDstMask(Ipv4Address network, Ipv4Mask mask)
  : m_network_ip(network), m_mask(mask)
{}


bool
FilterElementDstMask::match(Ptr<Packet> p) const
{
  
  Ptr<Packet> copy = p->Copy();
  // Peek the IPv4 header without consuming the packet

  PppHeader pppHeader;
  if(!copy->RemoveHeader(pppHeader)){
    return false;
  }

  Ipv4Header ipv4Header;
  if(!copy->RemoveHeader(ipv4Header)){
    return false;
  }

  Ipv4Address dst = ipv4Header.GetDestination();
  return dst.CombineMask(m_mask) == m_network_ip.CombineMask(m_mask) ? true : false;
}

}
