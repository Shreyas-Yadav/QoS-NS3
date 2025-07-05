/*
 * This program implements validation and setup for QoS mechanisms
 * including Strict Priority Queuing (SPQ) and Deficit Round Robin (DRR).
 */

#include <fstream>
#include <iostream>
#include "filter-element-dst-port.h"
#include "validation.h"

namespace ns3
{

/**
 * \brief Constructor
 */
Validation::Validation ()
  : spq (nullptr),
    drr (nullptr),
    m_clientNode (nullptr),
    m_routerNode (nullptr),
    m_serverNode (nullptr)
{
}

/**
 * \brief Parse configuration from JSON file
 * \param filename Path to the configuration file
 * \return true if parsing was successful, false otherwise
 */
bool
Validation::ParseConfigs (const std::string& filename)
{
  if (filename.empty ())
    {
      std::cerr << "Error: No configuration file specified" << std::endl;
      return false;
    }

  std::ifstream ifs (filename);
  if (!ifs.is_open ())
    {
      std::cerr << "Error: Could not open file " << filename << std::endl;
      return false;
    }

  nlohmann::json cfg;
  try
    {
      ifs >> cfg;
    }
  catch (const nlohmann::json::parse_error& e)
    {
      std::cerr << "JSON parse error: " << e.what () << std::endl;
      return false;
    }

  // Clear existing configuration
  m_configData = ConfigData ();

  // Store the name
  m_configData.name = cfg.value ("name", "");


  /**
   * \brief Process queue configurations from the JSON data
   */
  // Process queue configurations
  if (cfg.contains ("queues") && cfg["queues"].is_array ())
    {
      for (const auto& q : cfg["queues"])
        {
          QueueConfig queueConfig;

          // Common fields
          queueConfig.no = q.value ("no", 0);
          queueConfig.isDefault = q.value ("Default", false);
          queueConfig.maxPackets = q.value ("MaxPackets", 0u);
          queueConfig.destPort = q.value ("DestPort", 0u);
          queueConfig.srcPort = q.value ("SrcPort", 0u);
          queueConfig.protocol = q.value ("protocol", "");
          queueConfig.mask = q.value ("mask", "");
          queueConfig.destIp = q.value ("DestIp", "");
          queueConfig.sourceIp = q.value ("SourceIp", "");

          // Specific fields based on scheduler type
          if (m_configData.name == "spq")
            {
              queueConfig.priority = q.value ("Priority", 0u);
              queueConfig.weight = 0; // Not used in SPQ
            }
          else if (m_configData.name == "drr")
            {
              queueConfig.weight = q.value ("Weight", 0u);
              queueConfig.priority = 0; // Not used in DRR
            }

          m_configData.queues.push_back (queueConfig);
        }
    }

  // Initialize the appropriate scheduler based on the configuration
  if (m_configData.name == "spq")
    {
      // Initialize SPQ configuration
      spq = CreateObject<SPQ> ();
    }
  else if (m_configData.name == "drr")
    {
      // Initialize DRR configuration
      drr = CreateObject<DRR> ();
    }
  else
    {
      std::cerr << "Unknown scheduler type: " << m_configData.name << std::endl;
      return false;
    }

  return true;
}



/**
 * \brief Initialize the appropriate QoS mechanism based on configuration
 */
void
Validation::InitializeQoSMechanism ()
{
  if (m_configData.name == "spq")
    {
      InitializeSPQ ();
    }
  else if (m_configData.name == "drr")
    {
      InitializeDRR ();
    }
  else
    {
      std::cerr << "Cannot initialize unknown QoS mechanism: " << m_configData.name << std::endl;
    }
}

/**
 * \brief Initialize SPQ scheduler with traffic classes from configuration
 */
void
Validation::InitializeSPQ ()
{
  // Create SPQ scheduler if not already created
  if (!spq)
    {
      spq = CreateObject<SPQ> ();
    }

  // Create traffic classes for all queues in the configuration
  for (size_t i = 0; i < m_configData.queues.size (); i++)
    {
      TrafficClass* tc = new TrafficClass ();
      tc->SetMaxPackets (m_configData.queues[i].maxPackets);
      tc->SetPriority (m_configData.queues[i].priority);
      tc->SetIsDefault (m_configData.queues[i].isDefault);

      // Create a filter for this traffic class based on destination port
      Filter* filter = new Filter ();
      FilterElementDstPort* portFilter = new FilterElementDstPort (m_configData.queues[i].destPort);
      filter->AddFilterElement (portFilter);
      tc->AddFilter (filter);
      spq->AddClass (tc);
    }
}

/**
 * \brief Initialize DRR scheduler with traffic classes from configuration
 */
void
Validation::InitializeDRR ()
{
  // Create DRR scheduler if not already created
  if (!drr)
    {
      drr = CreateObject<DRR> ();
    }

  // Configure DRR using the stored configuration data
  for (const auto& queueConfig : m_configData.queues)
    {
      TrafficClass* tc = new TrafficClass ();
      tc->SetMaxPackets (queueConfig.maxPackets);
      tc->SetWeight (queueConfig.weight);

      Filter* filter = new Filter ();
      FilterElementDstPort* portFilter = new FilterElementDstPort (queueConfig.destPort);
      filter->AddFilterElement (portFilter);
      tc->AddFilter (filter);
      drr->AddClass (tc);
    }
}

/**
 * \brief Set up the network topology (client-router-server)
 */
void
Validation::InitializeTopology ()
{
  // Create nodes
  m_clientNode = CreateObject<Node> ();
  m_routerNode = CreateObject<Node> ();
  m_serverNode = CreateObject<Node> ();

  // Add nodes to container
  m_container = NodeContainer (m_clientNode, m_routerNode, m_serverNode);

  // Create point-to-point links
  m_p1Help.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  m_p1Help.SetChannelAttribute ("Delay", StringValue ("10ms"));

  m_p2Help.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  m_p2Help.SetChannelAttribute ("Delay", StringValue ("10ms"));

  // Install devices
  m_devices1 = m_p1Help.Install (NodeContainer (m_clientNode, m_routerNode));
  m_devices2 = m_p2Help.Install (NodeContainer (m_routerNode, m_serverNode));

  // Install Internet stack
  InternetStackHelper internet;
  internet.Install (m_container);

  // Assign IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  m_interfaces1 = ipv4.Assign (m_devices1);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  m_interfaces2 = ipv4.Assign (m_devices2);

  // Set up routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Ptr<PointToPointNetDevice> link1PtpNetworkDevice = m_routerNode->GetDevice(1)->GetObject<PointToPointNetDevice>();
}

/**
 * \brief Set up UDP applications based on the configuration
 */
void
Validation::InitializeUdpApplication ()
{
  // Common parameters
  uint32_t maxPacketSize = 1000;
  Time interPacketInterval = Seconds (0.002);


  Ptr<PointToPointNetDevice> clientRouter = m_routerNode->GetDevice (1)->GetObject<PointToPointNetDevice> ();
  
  if (m_configData.name == "spq")
    {
      clientRouter->SetQueue (spq);

      UdpServerHelper server1 (m_configData.queues[0].destPort);
      UdpServerHelper server2 (m_configData.queues[1].destPort);

      // Install servers with synchronized timing
      auto apps1 = server1.Install (m_serverNode);
      apps1.Start (Seconds (0.0));
      apps1.Stop (Seconds(40.0));

      auto apps2 = server2.Install (m_serverNode);
      apps2.Start (Seconds (0.0));
      apps2.Stop (Seconds(40.0));

      UdpClientHelper client1 (m_interfaces2.GetAddress (1), m_configData.queues[0].destPort);
      client1.SetAttribute ("MaxPackets", UintegerValue (m_configData.queues[0].maxPackets));
      client1.SetAttribute ("Interval", TimeValue (interPacketInterval));
      client1.SetAttribute ("PacketSize", UintegerValue (maxPacketSize));

      UdpClientHelper client2 (m_interfaces2.GetAddress (1), m_configData.queues[1].destPort);
      client2.SetAttribute ("MaxPackets", UintegerValue (m_configData.queues[1].maxPackets));
      client2.SetAttribute ("Interval", TimeValue (interPacketInterval));
      client2.SetAttribute ("PacketSize", UintegerValue (maxPacketSize));

      // Install clients with synchronized timing
      // Start client1 (port 10000) at 15s and stop at 30s
      apps1 = client1.Install (m_clientNode);
      apps1.Start (Seconds (15.0));
      apps1.Stop (Seconds (30.0));

      // Start client2 (port 10001) at 0s and stop at 40s
      apps2 = client2.Install (m_clientNode);
      apps2.Start (Seconds (0.0));
      apps2.Stop (Seconds(40.0));

      // Enable packet capture for analysis
      m_p1Help.EnablePcap ("scratch/final-project/Pre_SPQ", m_devices1.Get (1));
      m_p2Help.EnablePcap ("scratch/final-project/Post_SPQ", m_devices2.Get (0));
    }
  else if (m_configData.name == "drr")
    {
      clientRouter->SetQueue (drr);

      // DRR: all clients start together at time 0
      for (int i = 0; i < 3; i++)
        {
          UdpServerHelper server (m_configData.queues[i].destPort);
          auto app = server.Install (m_serverNode);
          app.Start (Seconds (0.0));
          app.Stop (Seconds (40.0));

          UdpClientHelper client (m_interfaces2.GetAddress (1), m_configData.queues[i].destPort);
          client.SetAttribute ("MaxPackets", UintegerValue (m_configData.queues[i].maxPackets));
          client.SetAttribute ("Interval", TimeValue (interPacketInterval));
          client.SetAttribute ("PacketSize", UintegerValue (maxPacketSize));
          auto cApp = client.Install (m_clientNode);
          cApp.Start (Seconds (0.0));
          cApp.Stop (Seconds (40.0));
        }

      // Enable packet capture for analysis
      m_p1Help.EnablePcap ("scratch/final-project/Pre_DRR", m_devices1.Get (1));
      m_p2Help.EnablePcap ("scratch/final-project/Post_DRR", m_devices2.Get (0));
    }
  else
    {
      std::cerr << "Unknown QoS for UDP applications: " << m_configData.name << std::endl;
    }

}

} 