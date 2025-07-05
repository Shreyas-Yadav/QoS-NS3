// File: filter-element-protocol.cc

#include "filter-element-protocol.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/ppp-header.h"

namespace ns3 {



FilterElementProtocol::FilterElementProtocol(uint8_t protocol)
  : m_protocol(protocol)
{}

bool
FilterElementProtocol::match(Ptr<Packet> p) const
{
  
  Ptr<Packet> copy = p->Copy();
  
  PppHeader pppHeader;
  if(!copy->RemoveHeader(pppHeader)){
    return false;
  }

  Ipv4Header ipv4Header;
  // Peek at the IPv4 header without consuming the packet
  if (!copy->RemoveHeader(ipv4Header)) {
    return false;
  }
  
  uint8_t proto = ipv4Header.GetProtocol();  
  return proto == m_protocol;
}

} // namespace ns3
