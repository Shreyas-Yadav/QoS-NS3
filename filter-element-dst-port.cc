/*
 * This program implements a filter element that matches packets
 * based on their TCP/UDP destination port.
 */

#include "filter-element-dst-port.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"

namespace ns3 {


/**
 * \brief Constructor
 * \param port Port number to match against packet destination port
 */
FilterElementDstPort::FilterElementDstPort (uint16_t port) 
  : m_port (port)
{
}

/**
 * \brief Check if packet's destination port matches the configured port
 * \param pkt The packet to check
 * \return true if the packet's transport destination port equals m_port
 */
bool
FilterElementDstPort::match (Ptr<Packet> pkt) const
{
  // Make a copy of the packet to avoid modifying the original
  Ptr<Packet> packetCopy = pkt->Copy ();
  
  // First check for PPP header
  PppHeader pppHeader;
  if (!packetCopy->RemoveHeader (pppHeader))
    {
      return false;
    }
  
  // Then check for IPv4 header
  Ipv4Header ipv4Header;
  if (!packetCopy->RemoveHeader (ipv4Header))
    {
      return false;
    }

  // Get the protocol type from the IPv4 header
  uint8_t protocol = ipv4Header.GetProtocol ();

  // If TCP, check TCP header
  if (protocol == 6) // TCP protocol number is 6
    {
      TcpHeader tcpHeader;

      // Ensure we can peek the TCP header
      if (!packetCopy->PeekHeader (tcpHeader))
        {
          return false;
        }
      
      // Check if the destination port in the TCP header matches
      return tcpHeader.GetDestinationPort () == m_port;
    }
  // If UDP, check the UDP Header instead
  else if (protocol == 17) // UDP protocol number is 17
    {
      UdpHeader udpHeader;

      // Ensure we can peek the UDP header
      if (!packetCopy->PeekHeader (udpHeader))
        {
          return false;
        }

      // Check if the destination port in the UDP header matches
      return udpHeader.GetDestinationPort () == m_port;
    }

  // If not TCP or UDP, no match
  return false;
}

} // namespace ns3
