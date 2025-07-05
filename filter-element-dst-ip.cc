/*
 * This program implements a filter element that matches packets
 * based on their IPv4 destination address.
 */

#include "filter-element-dst-ip.h"
#include "ns3/log.h"
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"

namespace ns3 {

/**
 * \brief Constructor
 * \param dst IPv4 address to match against packet destination
 */
FilterElementDstIp::FilterElementDstIp (Ipv4Address dst)
  : m_dst (dst)
{
}

/**
 * \brief Check if packet's IPv4 destination matches the configured address
 * \param p The packet to check
 * \return true if the packet's IPv4 destination equals m_dst
 */
bool
FilterElementDstIp::match (Ptr<Packet> p) const
{
  // Create a copy of the packet to avoid modifying the original
  Ptr<Packet> copy = p->Copy ();
  
  // First check for PPP header
  PppHeader pppHeader;
  if (!copy->PeekHeader (pppHeader))
    {
      return false;
    }
  
  // Then check for IPv4 header
  Ipv4Header ipHeader;
  if (!copy->PeekHeader (ipHeader))
    {
      return false;
    }

  // Extract the destination IP address and compare with our target
  Ipv4Address dst = ipHeader.GetDestination ();
  
  // Return true if the destination matches our target
  return dst == m_dst;
}

}
