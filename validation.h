/*
 * This program implements validation and setup for QoS mechanisms
 * including Strict Priority Queuing (SPQ) and Deficit Round Robin (DRR).
 */

#pragma once

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

#include <nlohmann/json.hpp>
#include "spq.h"
#include "drr.h"
#include <vector>

namespace ns3 {

/**
 * \brief Structure to represent a queue configuration
 *
 * This structure holds all parameters for a single traffic class queue
 * including common parameters and algorithm-specific ones.
 */
struct QueueConfig
{
  int no;                  //!< Queue number/identifier
  bool isDefault;          //!< Whether this is the default queue
  uint32_t maxPackets;     //!< Maximum number of packets in queue
  uint16_t destPort;       //!< Destination port for filtering
  uint16_t srcPort;        //!< Source port for filtering
  std::string protocol;    //!< Protocol for filtering
  std::string mask;        //!< Network mask for filtering
  std::string destIp;      //!< Destination IP for filtering
  std::string sourceIp;    //!< Source IP for filtering
  
  // SPQ specific
  uint32_t priority;       //!< Priority level (lower value = higher priority)
  
  // DRR specific
  uint32_t weight;         //!< Weight for DRR scheduling
};

/**
 * \brief Structure to represent the entire QoS configuration
 */
struct ConfigData
{
  std::string name;                //!< QoS mechanism name ("spq" or "drr")
  std::vector<QueueConfig> queues; //!< List of queue configurations
};

/**
 * \brief Class for validating and setting up QoS simulations
 *
 * This class handles configuration parsing, network topology setup,
 * and initialization of QoS mechanisms and applications.
 */
class Validation
{
public:
  /**
   * \brief Constructor
   */
  Validation ();
  
  /**
   * \brief Parse configuration from file and store in configData
   * \param filename Path to the JSON configuration file
   * \return true if parsing was successful, false otherwise
   */
  bool ParseConfigs (const std::string& filename);
  
  /**
   * \brief Initialize the appropriate QoS mechanism based on configuration
   */
  void InitializeQoSMechanism ();

  /**
   * \brief Initialize SPQ scheduler with traffic classes from configuration
   */
  void InitializeSPQ ();
  
  /**
   * \brief Initialize DRR scheduler with traffic classes from configuration
   */
  void InitializeDRR ();

  /**
   * \brief Set up the network topology (client-router-server)
   */
  void InitializeTopology ();
  
  /**
   * \brief Set up UDP applications based on the configuration
   */
  void InitializeUdpApplication ();

  /**
   * \brief Get the name of the configured QoS mechanism
   * \return Name of the QoS mechanism ("spq" or "drr")
   */
  std::string GetName () const { return m_configData.name; }
  
  /**
   * \brief Set the name of the QoS mechanism
   * \param name Name to set ("spq" or "drr")
   */
  void SetName (std::string& name) { m_configData.name = name; }

  /**
   * \brief Get the parsed configuration data
   * \return Reference to the configuration data
   */
  const ConfigData& GetConfigData () const { return m_configData; }

  // Queue scheduler instances
  Ptr<SPQ> spq; //!< SPQ scheduler instance
  Ptr<DRR> drr; //!< DRR scheduler instance

private:
  // Parsed configuration data
  ConfigData m_configData; //!< Parsed configuration data
  
  // Node and topology
  Ptr<Node> m_clientNode;  //!< Client node
  Ptr<Node> m_routerNode;  //!< Router node (with QoS)
  Ptr<Node> m_serverNode;  //!< Server node
  NodeContainer m_container; //!< Container for all nodes
  NetDeviceContainer m_devices1; //!< Devices for client-router link
  NetDeviceContainer m_devices2; //!< Devices for router-server link
  Ipv4InterfaceContainer m_interfaces1; //!< Interfaces for client-router link
  Ipv4InterfaceContainer m_interfaces2; //!< Interfaces for router-server link

  // Link helpers
  PointToPointHelper m_p1Help; //!< Helper for client-router link
  PointToPointHelper m_p2Help; //!< Helper for router-server link
};

} // namespace ns3