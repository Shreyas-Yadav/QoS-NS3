/*
 * This program implements a traffic class for Quality of Service (QoS)
 * packet scheduling mechanisms.
 */

#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include "filter.h"
#include "ns3/packet.h"
#include <queue>

namespace ns3 {

/**
 * \brief A traffic class for QoS packet scheduling
 *
 * This class represents a traffic class that can be used with
 * different QoS scheduling algorithms. It maintains a queue of
 * packets and has associated parameters like priority and weight.
 */
class TrafficClass 
{
public:
  /**
   * \brief Constructor
   */
  TrafficClass ();
  
  /**
   * \brief Virtual destructor
   */
  virtual ~TrafficClass () = default;

  /**
   * \brief Enqueue a packet into this traffic class
   * \param p Packet to enqueue
   * \return true if the packet was successfully enqueued
   */
  bool Enqueue (Ptr<Packet> p);
  
  /**
   * \brief Dequeue a packet from this traffic class
   * \return The dequeued packet, or nullptr if the queue is empty
   */
  Ptr<Packet> Dequeue ();
  
  /**
   * \brief Peek at the next packet without removing it
   * \return The next packet, or nullptr if the queue is empty
   */
  Ptr<Packet> Peek () const;
  
  /**
   * \brief Check if the traffic class queue is empty
   * \return true if the queue is empty, false otherwise
   */
  bool IsEmpty () const;
  
  /**
   * \brief Check if a packet matches this traffic class's filters
   * \param p Packet to check
   * \return true if the packet matches, false otherwise
   */
  bool Match (Ptr<Packet> p) const;

  /**
   * \brief Get the current number of packets in the queue
   * \return Number of packets
   */
  uint32_t GetPackets () const;

  /**
   * \brief Set the maximum number of packets allowed in the queue
   * \param maxPackets Maximum number of packets
   */
  void SetMaxPackets (uint32_t maxPackets);
  
  /**
   * \brief Get the maximum number of packets allowed in the queue
   * \return Maximum number of packets
   */
  uint32_t GetMaxPackets () const;

  /**
   * \brief Set the weight for this traffic class (used in DRR)
   * \param weight Weight value
   */
  void SetWeight (double weight);
  
  /**
   * \brief Get the weight for this traffic class
   * \return Weight value
   */
  double GetWeight () const;

  /**
   * \brief Set the priority for this traffic class (used in SPQ)
   * \param priority Priority value (lower value = higher priority)
   */
  void SetPriority (uint32_t priority);
  
  /**
   * \brief Get the priority for this traffic class
   * \return Priority value
   */
  uint32_t GetPriority () const;

  /**
   * \brief Set the filters for this traffic class
   * \param filters Vector of filters
   */
  void SetFilters (const std::vector<Filter*>& filters);
  
  /**
   * \brief Get the filters for this traffic class
   * \return Vector of filters
   */
  std::vector<Filter*> GetFilters () const;
  
  /**
   * \brief Add a filter to this traffic class
   * \param filter Filter to add
   */
  void AddFilter (Filter* filter);

  /**
   * \brief Set whether this is the default traffic class
   * \param isDefault true if this is the default traffic class
   */
  void SetIsDefault (bool isDefault);
  
  /**
   * \brief Check if this is the default traffic class
   * \return true if this is the default traffic class
   */
  bool IsDefault () const;

private:
  std::queue<Ptr<Packet>> m_queue;     //!< Queue of packets
  std::vector<Filter*> m_filters;      //!< Filters for this traffic class

  uint32_t m_packets;                  //!< Current number of packets in the queue
  uint32_t m_maxPackets;               //!< Maximum number of packets allowed
  double m_weight;                     //!< Weight for DRR scheduling
  uint32_t m_priority;                 //!< Priority for SPQ scheduling
  bool m_isDefault;                    //!< Whether this is the default traffic class
};

} // namespace ns3

#endif // TRAFFIC_CLASS_H
