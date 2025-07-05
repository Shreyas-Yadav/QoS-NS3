/*
 * This program implements a traffic class for Quality of Service (QoS)
 * packet scheduling mechanisms.
 */

#include "ns3/core-module.h"
#include "ns3/log.h"
#include "filter.h"
#include "traffic-class.h"
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"

namespace ns3 {

/**
 * \brief Constructor - initializes default values
 */
TrafficClass::TrafficClass () 
  : m_packets (0), 
    m_maxPackets (100), 
    m_weight (0), 
    m_priority (0),
    m_isDefault (false)
{
}

/**
 * \brief Set whether this is the default traffic class
 * \param isDefault true if this is the default traffic class
 */
void 
TrafficClass::SetIsDefault (bool isDefault)
{
  m_isDefault = isDefault;
}

/**
 * \brief Check if this is the default traffic class
 * \return true if this is the default traffic class
 */
bool 
TrafficClass::IsDefault () const
{
  return m_isDefault;
}

/**
 * \brief Enqueue a packet into this traffic class
 * \param pkt Packet to enqueue
 * \return true if the packet was successfully enqueued
 */
bool 
TrafficClass::Enqueue (Ptr<Packet> pkt)
{
  // Ensure the packet count is less than max
  if (m_packets < m_maxPackets)
    {
      m_queue.push (pkt);
      m_packets++;
      return true;
    }

  return false;
}

/**
 * \brief Dequeue a packet from this traffic class
 * \return The dequeued packet, or nullptr if the queue is empty
 */
Ptr<Packet> 
TrafficClass::Dequeue ()
{
  // Return null pointer on empty queue
  if (m_queue.empty ())
    {
      std::cout << "Be aware that the queue is empty." << std::endl;
      return nullptr;
    }

  // Dequeue the packet and update the packet count
  Ptr<Packet> pkt = m_queue.front ();
  
  // Pop the packet from the queue and decrement the packet count
  m_queue.pop ();
  m_packets--;
  
  return pkt;
}

/**
 * \brief Check if the traffic class queue is empty
 * \return true if the queue is empty, false otherwise
 */
bool 
TrafficClass::IsEmpty () const
{
  return m_packets == 0;
}

/**
 * \brief Get the current number of packets in the queue
 * \return Number of packets
 */
uint32_t
TrafficClass::GetPackets () const
{
  return m_packets;
}

/**
 * \brief Get the filters for this traffic class
 * \return Vector of filters
 */
std::vector<Filter*> 
TrafficClass::GetFilters () const
{
  return m_filters;
}

/**
 * \brief Set the filters for this traffic class
 * \param filters Vector of filters
 */
void 
TrafficClass::SetFilters (const std::vector<Filter*>& filters)
{
  m_filters.clear ();
  m_filters = filters;
}

/**
 * \brief Get the maximum number of packets allowed in the queue
 * \return Maximum number of packets
 */
uint32_t
TrafficClass::GetMaxPackets () const
{
  return m_maxPackets;
}

/**
 * \brief Peek at the next packet without removing it
 * \return The next packet, or nullptr if the queue is empty
 */
Ptr<Packet> 
TrafficClass::Peek () const
{
  if (IsEmpty ())
    {
      return nullptr;
    }
  
  Ptr<Packet> pkt = m_queue.front ();
  return pkt;
}

/**
 * \brief Add a filter to this traffic class
 * \param filter Filter to add
 */
void 
TrafficClass::AddFilter (Filter *filter)
{
  m_filters.push_back (filter);
}

/**
 * \brief Set the maximum number of packets allowed in the queue
 * \param maxPackets Maximum number of packets
 */
void 
TrafficClass::SetMaxPackets (uint32_t maxPackets)
{
  m_maxPackets = maxPackets;
}

/**
 * \brief Set the weight for this traffic class (used in DRR)
 * \param weight Weight value
 */
void 
TrafficClass::SetWeight (double weight)
{
  m_weight = weight;
}

/**
 * \brief Get the weight for this traffic class
 * \return Weight value
 */
double 
TrafficClass::GetWeight () const
{
  return m_weight;
}

/**
 * \brief Set the priority for this traffic class (used in SPQ)
 * \param priority Priority value (lower value = higher priority)
 */
void 
TrafficClass::SetPriority (uint32_t priority)
{
  m_priority = priority;
}

/**
 * \brief Get the priority for this traffic class
 * \return Priority value
 */
uint32_t 
TrafficClass::GetPriority () const
{
  return m_priority;
}

/**
 * \brief Check if a packet matches this traffic class's filters
 * \param pkt Packet to check
 * \return true if the packet matches, false otherwise
 */
bool 
TrafficClass::Match (Ptr<Packet> pkt) const
{
  // If no filters, match everything
  if (m_filters.empty ())
    {
      return true;
    }

  // Check if the packet matches any of the filters (OR logic)
  for (Filter *filter : m_filters)
    {
      bool matches = filter->Match (pkt);
      if (matches)
        {
          return true;
        }
    }

  // No matching filters found
  return false;
}

} // namespace ns3