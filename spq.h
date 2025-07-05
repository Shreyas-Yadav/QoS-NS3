/*
 * This program implements a Strict Priority Queuing (SPQ) scheduler
 * for Quality of Service (QoS) packet scheduling.
 */

#ifndef SPQ_H
#define SPQ_H

#include "diff-serve.h"

namespace ns3 {

/**
 * \ingroup diffserv
 * \brief Strict Priority Queuing (SPQ) scheduler extending DiffServ
 *
 * This class implements the SPQ scheduling algorithm, which always
 * serves packets from the highest priority queue that has packets.
 * Lower priority queues are only served when all higher priority
 * queues are empty.
 */
class SPQ : public DiffServ 
{
public:
  /**
   * \brief Constructor
   */
  SPQ ();
  
  /**
   * \brief Destructor
   */
  ~SPQ () = default;
  
  /**
   * \brief Select the next packet to be dequeued based on SPQ algorithm
   * \return Packet at the front of the highest priority non-empty queue
   *
   * This method implements the core SPQ scheduling algorithm, which
   * always selects packets from the highest priority queue that has packets.
   */
  virtual Ptr<const Packet> Schedule () const override;
};

} // namespace ns3

#endif // SPQ_H