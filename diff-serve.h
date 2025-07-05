/*
 * This program implements a Differentiated Services (DiffServ) base class
 * for Quality of Service (QoS) mechanisms.
 */

#ifndef DIFFSERV_H
#define DIFFSERV_H

#include "ns3/queue.h"
#include "ns3/packet.h"
#include <vector>
#include "traffic-class.h"

namespace ns3 {

/**
 * \brief Base class for Differentiated Services (DiffServ) QoS mechanisms
 *
 * This class provides the foundation for implementing various QoS scheduling
 * algorithms like Strict Priority Queuing (SPQ) and Deficit Round Robin (DRR).
 * It manages multiple traffic classes and handles packet classification.
 */
class DiffServ : public Queue<Packet>
{
public:
  /**
   * \brief Constructor
   */
  DiffServ ();
  
  /**
   * \brief Destructor
   */
  ~DiffServ ();
  
  /**
   * \brief Enqueue a packet into the appropriate traffic class
   * \param p Packet to enqueue
   * \return true if the packet was successfully enqueued
   */
  bool Enqueue (Ptr<Packet> p) override;
  
  /**
   * \brief Dequeue a packet based on the scheduling algorithm
   * \return The dequeued packet, or nullptr if no packet is available
   */
  Ptr<Packet> Dequeue () override;
  
  /**
   * \brief Remove a packet from the queue
   * \return The removed packet, or nullptr if no packet is available
   */
  Ptr<Packet> Remove () override;
  
  /**
   * \brief Peek at the next packet to be dequeued without removing it
   * \return The next packet to be dequeued, or nullptr if no packet is available
   */
  Ptr<const Packet> Peek () const override;
  
  /**
   * \brief Get all traffic classes
   * \return Vector of traffic classes
   */
  std::vector<TrafficClass*> GetClasses () const;
  
  /**
   * \brief Schedule the next packet to be dequeued
   * \return The next packet to be dequeued, or nullptr if no packet is available
   *
   * This is a pure virtual function that must be implemented by derived classes
   * to define their specific scheduling algorithm.
   */
  virtual Ptr<const Packet> Schedule () const = 0;
  
  /**
   * \brief Classify a packet into a traffic class
   * \param p Packet to classify
   * \return Index of the traffic class that matches the packet, or -1 if no match
   */
  virtual int Classify (Ptr<Packet> p);
  
  /**
   * \brief Add a traffic class to the DiffServ queue
   * \param trafficClass Pointer to the traffic class to add
   */
  virtual void AddClass (TrafficClass* trafficClass);
  
private:
  std::vector<TrafficClass*> m_queueClasses; //!< Traffic classes
  
  /**
   * \brief Internal implementation of Enqueue
   * \param p Packet to enqueue
   * \return true if the packet was successfully enqueued
   */
  bool DoEnqueue (Ptr<Packet> p);
  
  /**
   * \brief Internal implementation of Dequeue
   * \return The dequeued packet, or nullptr if no packet is available
   */
  Ptr<Packet> DoDequeue ();
  
  /**
   * \brief Internal implementation of Peek
   * \return The next packet to be dequeued, or nullptr if no packet is available
   */
  Ptr<const Packet> DoPeek () const;
  
  /**
   * \brief Internal implementation of Remove
   * \return The removed packet, or nullptr if no packet is available
   */
  Ptr<Packet> DoRemove ();
};

} // namespace ns3

#endif // DIFFSERV_H
