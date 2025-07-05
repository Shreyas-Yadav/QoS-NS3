/*
 * This program implements a Differentiated Services (DiffServ) base class
 * for Quality of Service (QoS) mechanisms.
 */

#include "diff-serve.h"
#include "traffic-class.h"
#include "filter.h"
#include <iostream>
#include "ns3/ipv4-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"

namespace ns3 {

/**
 * \brief Constructor
 */
DiffServ::DiffServ () 
{
}

/**
 * \brief Destructor - cleans up all traffic classes
 */
DiffServ::~DiffServ () 
{
  // Delete all traffic classes
  for (auto tc : m_queueClasses) 
    {
      delete tc;
    }
  m_queueClasses.clear ();
}

/**
 * \brief Add a traffic class to the DiffServ queue
 * \param trafficClass Pointer to the traffic class to add
 */
void 
DiffServ::AddClass (TrafficClass* trafficClass) 
{
  m_queueClasses.push_back (trafficClass);
}

/**
 * \brief Enqueue a packet into the appropriate traffic class
 * \param p Packet to enqueue
 * \return true if the packet was successfully enqueued
 */
bool 
DiffServ::Enqueue (Ptr<Packet> p) 
{
  return DoEnqueue (p);
}

/**
 * \brief Dequeue a packet based on the scheduling algorithm
 * \return The dequeued packet, or nullptr if no packet is available
 */
Ptr<Packet> 
DiffServ::Dequeue () 
{
  return DoDequeue ();
}

/**
 * \brief Peek at the next packet to be dequeued without removing it
 * \return The next packet to be dequeued, or nullptr if no packet is available
 */
Ptr<const Packet> 
DiffServ::Peek () const
{
  return DoPeek ();
}

/**
 * \brief Remove a packet from the queue
 * \return The removed packet, or nullptr if no packet is available
 */
Ptr<Packet> 
DiffServ::Remove () 
{
  return DoRemove ();
}

/**
 * \brief Get all traffic classes
 * \return Vector of traffic classes
 */
std::vector<TrafficClass*> 
DiffServ::GetClasses () const
{
  return m_queueClasses;
}

/**
 * \brief Internal implementation of Enqueue
 * \param packet Packet to enqueue
 * \return true if the packet was successfully enqueued
 */
bool 
DiffServ::DoEnqueue (Ptr<Packet> packet) 
{
  // Classify the packet to determine which traffic class it belongs to
  int classIndex = Classify (packet);
  
  // If a valid traffic class was found, enqueue the packet
  if (classIndex > -1 && classIndex < int(m_queueClasses.size ())) 
    {
      bool result = m_queueClasses[classIndex]->Enqueue (packet);
      return result;
    }
  
  // No valid traffic class found
  return false;
}

/**
 * \brief Internal implementation of Dequeue
 * \return The dequeued packet, or nullptr if no packet is available
 */
Ptr<Packet> 
DiffServ::DoDequeue () 
{
  // Schedule() returns the next packet to dequeue based on the scheduling algorithm
  Ptr<const Packet> packet = Schedule ();
  if (packet == nullptr) 
    {
      return nullptr;
    }
  
  // Find which traffic class this packet belongs to
  for (size_t i = 0; i < m_queueClasses.size (); i++) 
    {
      if (!m_queueClasses[i]->IsEmpty () && m_queueClasses[i]->Peek () == packet) 
        {
          // Found the traffic class, dequeue from it
          return m_queueClasses[i]->Dequeue ();
        }
    }
  
  // If we couldn't find the traffic class (shouldn't happen), return nullptr
  return nullptr;
}

/**
 * \brief Internal implementation of Remove
 * \return The removed packet, or nullptr if no packet is available
 */
Ptr<Packet> 
DiffServ::DoRemove () 
{
  Ptr<const Packet> packet = Schedule ();
  if (packet == nullptr) 
    {
      return nullptr;
    }
  
  // Find which traffic class this packet belongs to
  for (size_t i = 0; i < m_queueClasses.size (); i++) 
    {
      if (!m_queueClasses[i]->IsEmpty () && m_queueClasses[i]->Peek () == packet) 
        {
          // Found the traffic class, dequeue from it
          return m_queueClasses[i]->Dequeue ();
        }
    }
  
  // If we couldn't find the traffic class (shouldn't happen), return nullptr
  return nullptr;
}

/**
 * \brief Internal implementation of Peek
 * \return The next packet to be dequeued, or nullptr if no packet is available
 */
Ptr<const Packet> 
DiffServ::DoPeek () const 
{
  Ptr<const Packet> pkt = Schedule ();
  return pkt != nullptr ? pkt->Copy () : nullptr;
}

/**
 * \brief Classify a packet into a traffic class
 * \param packet Packet to classify
 * \return Index of the traffic class that matches the packet, or -1 if no match
 */
int 
DiffServ::Classify (Ptr<Packet> packet) 
{
  int defaultIndex = -1;

  // Try to match the packet against each traffic class's filters
  for (uint32_t i = 0; i < m_queueClasses.size (); i++)
    {
      // If the packet matches this traffic class's filters, return its index
      if (m_queueClasses[i]->Match (packet))
        {
          return i;
        }
      
      // Keep track of the default traffic class (if any)
      if (m_queueClasses[i]->IsDefault ())
        {
          defaultIndex = i;
        }
    }

  // If no match was found, return the default traffic class index (or -1 if none)
  return defaultIndex;
}

}
