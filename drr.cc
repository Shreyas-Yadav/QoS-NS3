/*
 * This program implements a Deficit Round Robin (DRR) scheduler
 * for Quality of Service (QoS) packet scheduling.
 */

#include "drr.h"
#include <iostream>

namespace ns3 {

/**
 * \brief Constructor - initializes the current queue index to 0
 */
DRR::DRR ()
  : m_currentQueueIndex (0)
{
}

/**
 * \brief Dequeue the packet selected by the DRR scheduling logic
 * \return Dequeued packet or nullptr if none available
 *
 * This method dequeues the next scheduled packet and then updates
 * the m_currentQueueIndex and m_creditBalance to reflect the dequeue operation.
 */
Ptr<Packet>
DRR::Dequeue ()
{
  Ptr<Packet> dequeuedPacket = DiffServ::Dequeue ();

  if (dequeuedPacket)
    {
      // Update the current queue index and credit balance after successful dequeue
      m_currentQueueIndex = m_scheduledQueueIndex;
      m_creditBalance = m_tempCreditBalance;
      return dequeuedPacket;
    }

  return nullptr;
}

/**
 * \brief Remove a packet from the queue
 * \return The removed packet, or nullptr if no packet is available
 */
Ptr<Packet>
DRR::Remove ()
{
  std::vector<TrafficClass*> queueList = GetClasses ();
  
  // Check if the scheduled queue is valid and not empty
  if (m_scheduledQueueIndex >= queueList.size () || queueList[m_scheduledQueueIndex]->IsEmpty ())
    {
      std::cout << "Active queue is invalid or empty during Remove." << std::endl;
      return nullptr;
    }

  // Remove the packet from the scheduled queue
  Ptr<Packet> packetToRemove = queueList[m_scheduledQueueIndex]->Dequeue ();

  if (packetToRemove != nullptr)
    {
      std::cout << "Got removed: " << packetToRemove->GetSize () << std::endl;
      std::cout << "Removed from queue index: " << m_scheduledQueueIndex << std::endl;

      // Update the current queue index and credit balance after successful removal
      m_currentQueueIndex = m_scheduledQueueIndex;
      m_creditBalance = m_tempCreditBalance;
    }

  return packetToRemove;
}

/**
 * \brief Select the next packet to be dequeued based on DRR algorithm
 * \return Packet at the front of the scheduled queue
 *
 * This method implements the core DRR scheduling algorithm, which
 * distributes bandwidth fairly among traffic classes based on their weights.
 */
Ptr<const Packet>
DRR::Schedule () const
{
  // Get traffic class queues
  std::vector<TrafficClass*> queueList = GetClasses ();

  // Check that there are queues to serve
  if (queueList.empty ())
    {
    //   std::cout << "No queues to serve." << std::endl;
      return nullptr;
    }

  // Reset to current deficit
  m_scheduledQueueIndex = m_currentQueueIndex;
  m_tempCreditBalance = m_creditBalance;

  // Check first that there are any packets in any queue
  uint32_t emptyQueueCount = 0;
  for (uint32_t i = 0; i < queueList.size (); i++)
    {
      if (queueList[i]->IsEmpty ())
        {
          emptyQueueCount++;
        }
    }
  
  if (emptyQueueCount == queueList.size ())
    {
    //   std::cout << "All queues are empty." << std::endl;
      return nullptr;
    }

  // DRR algorithm implementation
  while (true)
    {
      if (!queueList[m_scheduledQueueIndex]->IsEmpty ())
        {
          // Add the queue's weight to its credit balance
          m_tempCreditBalance[m_scheduledQueueIndex] =
            queueList[m_scheduledQueueIndex]->GetWeight () +
            m_tempCreditBalance[m_scheduledQueueIndex];

          // Check if the queue has enough credit to send the packet
          uint32_t packetLength = queueList[m_scheduledQueueIndex]->Peek ()->GetSize ();
          if (packetLength <= m_tempCreditBalance[m_scheduledQueueIndex])
            {
              // Deduct the packet size from the credit balance
              m_tempCreditBalance[m_scheduledQueueIndex] -= packetLength;
              return queueList[m_scheduledQueueIndex]->Peek ();
            }
        }
      
      // Move to the next queue in round-robin fashion
      m_scheduledQueueIndex = (m_scheduledQueueIndex + 1) % queueList.size ();
    }
  
  // This line should never be reached due to the infinite loop above
  return nullptr;
}

/**
 * \brief Add a new TrafficClass to the DRR queue system
 * \param trafficClass Pointer to the TrafficClass instance
 *
 * This method adds a traffic class and initializes its credit balance to zero.
 */
void
DRR::AddClass (TrafficClass* trafficClass)
{
  DiffServ::AddClass (trafficClass);
  m_creditBalance.push_back (0);
}

} 