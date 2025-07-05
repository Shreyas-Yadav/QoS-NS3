/*
 * This program implements a packet filter system that combines
 * multiple filter elements with AND logic.
 */

#include "filter.h"
#include "filter-element.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/packet.h"

namespace ns3 {

/**
 * \brief Destructor implementation
 */
Filter::~Filter () = default;

/**
 * \brief Get all filter elements
 * \return Vector of filter elements
 */
std::vector<FilterElement*> 
Filter::GetFilters ()
{
  return m_elements;
} 

/**
 * \brief Add a filter element to this filter
 * \param element The filter element to add
 */
void
Filter::AddFilterElement (FilterElement* element)
{
  m_elements.push_back (element);
}

/**
 * \brief Check if a packet matches all filter elements
 * \param packet The packet to check
 * \return true only if all filter elements match, false otherwise
 */
bool
Filter::Match (Ptr<Packet> packet) const
{
  // If no filter elements, match everything
  if (m_elements.empty ())
    {
      return true;
    }

  // Check each filter element - all must match (AND logic)
  for (const auto& e : m_elements)
    {
      bool matches = e->match (packet);
      if (!matches)
        {
          return false;
        }
    }
  
  // All filter elements matched
  return true;
}

} // namespace ns3
