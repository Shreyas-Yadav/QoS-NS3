/*
 * This program implements a Deficit Round Robin (DRR) scheduler
 * for Quality of Service (QoS) packet scheduling.
 */

#ifndef DRR_H
#define DRR_H

#include "diff-serve.h"
#include <vector>

namespace ns3 {

/**
 * \ingroup diffserv
 * \brief Deficit Round Robin (DRR) scheduler extending DiffServ
 *
 * This class implements the DRR scheduling algorithm, which distributes
 * bandwidth across multiple traffic classes according to their weight.
 * Each traffic class receives service proportional to its configured weight.
 */
class DRR : public DiffServ
{
public:
  /**
   * \brief Constructor
   */
  DRR ();

  /**
   * \brief Dequeue the packet selected by the DRR scheduling logic
   * \return Dequeued packet or nullptr if none available
   */
  Ptr<Packet> Dequeue () override;

  /**
   * \brief Remove a packet from the queue
   * \return The removed packet, or nullptr if no packet is available
   */
  Ptr<Packet> Remove () override;

  /**
   * \brief Select the next packet to be dequeued based on DRR algorithm
   * \return Packet at the front of the scheduled queue
   *
   * This method implements the core DRR scheduling algorithm, which
   * distributes bandwidth fairly among traffic classes based on their weights.
   */
  Ptr<const Packet> Schedule () const override;

  /**
   * \brief Add a new TrafficClass to the DRR queue system
   * \param trafficClass Pointer to the TrafficClass instance
   *
   * This method adds a traffic class and initializes its credit balance to zero.
   */
  void AddClass (TrafficClass* trafficClass) override;

private:
  u_int32_t m_baseQuantum;
  uint32_t m_currentQueueIndex;              //!< Currently active queue index
  mutable uint32_t m_scheduledQueueIndex;    //!< Candidate queue during scheduling
  std::vector<uint32_t> m_creditBalance;     //!< Deficit counters for each queue
  mutable std::vector<uint32_t> m_tempCreditBalance; //!< Working copy during Schedule
};

} // namespace ns3

#endif // DRR_H