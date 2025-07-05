#include "filter-element-src-ip.h"
#include "ns3/ppp-header.h"
#include <iostream>

namespace ns3 {

FilterElementSrcIp::FilterElementSrcIp(Ipv4Address src)
  : m_src(src)
{}



bool
FilterElementSrcIp::match(Ptr<Packet> p) const
{
  // Peek at the IPv4 header without removing it
  Ptr<Packet> copy = p->Copy();
  PppHeader pppHeader;

  if(!copy->RemoveHeader(pppHeader)){
    return false;
  }

  Ipv4Header ipHeader;
  
  // Check if the packet contains an IPv4 header
  if(!copy->PeekHeader(ipHeader)){
    return false;
  }

  Ipv4Address src = ipHeader.GetSource();
  return src == m_src;

}

} // namespace ns3
