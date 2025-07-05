// File: filter-element-src-port.cc

#include "filter-element-src-port.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"
namespace ns3 {



FilterElementSrcPort::FilterElementSrcPort(uint16_t port)
  : m_port(port)
{}


bool
FilterElementSrcPort::match(Ptr<Packet> p) const
{


  // Make a copy to avoid mutating the original packet
  Ptr<Packet> copy = p->Copy();

  PppHeader pppHeader;
  if(!copy->RemoveHeader(pppHeader)){
    return false;
  }

  Ipv4Header ipHeader;
  if(!copy->RemoveHeader(ipHeader)){
    return false;
  }


  uint8_t protocol = ipHeader.GetProtocol();

  if(protocol == 17){
    UdpHeader udpheader;
    if(!copy->RemoveHeader(udpheader)){
      return false;
    }
    uint16_t port = udpheader.GetDestinationPort();
    return port == m_port;
  }
  else if(protocol == 6){
    TcpHeader tcpHeader;
    if(!copy->RemoveHeader(tcpHeader)){
      return false;
    }
    uint16_t port = tcpHeader.GetDestinationPort();
    return port == m_port;
  }

  return false;

}

} // namespace ns3
