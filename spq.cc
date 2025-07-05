/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program implements a Strict Priority Queuing (SPQ) scheduler
 * for Quality of Service (QoS) packet scheduling.
 */

#include "spq.h"
#include <iostream>
#include <limits>

namespace ns3 {

/**
 * \brief Constructor
 */
SPQ::SPQ () 
  : DiffServ ()
{
}

/**
 * \brief Select the next packet to be dequeued based on SPQ algorithm
 * \return Packet at the front of the highest priority non-empty queue
 *
 * This method implements the core SPQ scheduling algorithm, which
 * always selects packets from the highest priority queue that has packets.
 */
Ptr<const Packet> 
SPQ::Schedule () const
{
  // Get all traffic classes
  std::vector<TrafficClass*> classes = GetClasses ();
  
  // If no classes, return nullptr
  if (classes.empty ()) 
    {
      return nullptr;
    }
  
  // Find the highest priority class (lowest priority value) that has packets
  TrafficClass* highestPriorityClass = nullptr;
  uint32_t highestPriority = std::numeric_limits<uint32_t>::max ();
  
  for (size_t i = 0; i < classes.size (); ++i) 
    {
      auto tc = classes[i];
      bool isEmpty = tc->IsEmpty ();
      uint32_t priority = tc->GetPriority ();
      
      // If this class has packets and higher priority (lower value) than current best
      if (!isEmpty && priority < highestPriority) 
        {
          highestPriorityClass = tc;
          highestPriority = priority;
        }
    }
  
  // If we found a non-empty class, return its packet
  if (highestPriorityClass != nullptr) 
    {
      return highestPriorityClass->Peek ();
    }
  
  // No packets in any class
  // std::cout << "  No non-empty classes found" << std::endl;
  return nullptr;
}

} // namespace ns3