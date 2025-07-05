/*
 * This program implements a packet filter system that combines
 * multiple filter elements with AND logic.
 */

#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/packet.h"
#include "filter-element.h"

namespace ns3 {

/**
 * \brief A filter that combines multiple filter elements with AND logic
 *
 * This class represents a filter that can be used to match packets based on
 * multiple conditions. A packet matches the filter only if it matches all
 * of the filter elements.
 */
class Filter
{
public:
  /**
   * \brief Default constructor
   */
  Filter () = default;
  
  /**
   * \brief Destructor
   */
  ~Filter ();

  /**
   * \brief Add one primitive condition
   * \param element The filter element to add
   *
   * The Filter takes ownership of the pointer.
   */
  void AddFilterElement (FilterElement* element);

  /**
   * \brief Check if a packet matches all filter elements
   * \param packet The packet to check
   * \return true only if **all** FilterElement::match(packet) return true
   */
  bool Match (Ptr<Packet> packet) const;

  /**
   * \brief Get all filter elements
   * \return Vector of filter elements
   */
  std::vector<FilterElement*> GetFilters ();

private:
  std::vector<FilterElement*> m_elements;  //!< The filter elements
};

} // namespace ns3

#endif // FILTER_H
