/*
 * This program implements a base class for packet filter elements
 * that can be combined to create complex packet filters.
 */

#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H

#include "ns3/packet.h"

namespace ns3 {

/**
 * \brief Base class for filter elements that match specific packet attributes
 *
 * This class defines the interface for filter elements that can be combined
 * to create complex packet filters. Each filter element checks a specific
 * attribute of a packet (e.g., source IP, destination port).
 */
class FilterElement
{
public:
  /**
   * \brief Virtual destructor ensures proper cleanup of derived classes
   */
  virtual ~FilterElement () = default;

  /**
   * \brief Check if a packet matches this filter element's condition
   * \param p The packet to check
   * \return true if the packet satisfies this condition, false otherwise
   */
  virtual bool match (Ptr<Packet> p) const = 0;
};

} // namespace ns3

#endif // FILTER_ELEMENT_H
