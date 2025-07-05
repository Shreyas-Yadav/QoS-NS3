/*
 * This program implements a Quality of Service (QoS) simulation using
 * either Strict Priority Queuing (SPQ) or Deficit Round Robin (DRR)
 * scheduling algorithms. It reads configuration from a JSON file and
 * sets up a client-router-server topology to demonstrate packet
 * scheduling behavior.
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/udp-header.h"
#include "ns3/ipv4-header.h"
#include "nlohmann/json.hpp"

#include "validation.h"
#include <iostream>

namespace ns3 {

/**
 * \brief Main function for the QoS simulation
 *
 * This function initializes the simulation based on a configuration file
 * that specifies either SPQ or DRR as the QoS mechanism, along with
 * traffic class parameters.
 *
 * \param argc Number of command line arguments
 * \param argv Command line arguments
 * \return 0 on successful completion, 1 on error
 */
int
main(int argc, char *argv[])
{
  // Support both positional argument and --config flag
  std::string configFile;
  if (argc == 2)
    {
      // e.g., ./main spq_config.json
      configFile = argv[1];
    }
  else
    {
      CommandLine cmd (__FILE__);
      cmd.AddValue ("config", "Path to QoS JSON configuration file (e.g., spq_config.json or drr_config.json)", configFile);
      cmd.Parse (argc, argv);
    }

  if (configFile.empty ())
    {
      std::cerr << "Usage: " << argv[0] << " <config-file.json> OR --config=<config-file.json>" << std::endl;
      return 1;
    }

  // Create validator and parse configuration
  Validation validator;
  if (!validator.ParseConfigs (configFile))
    {
      std::cerr << "Error: Failed to parse configuration file: " << configFile << std::endl;
      return 1;
    }


  // Build the topology (client-router-server) and install protocols
  validator.InitializeTopology ();

  // Initialize the selected QoS mechanism (SPQ or DRR) with traffic classes and filters
  validator.InitializeQoSMechanism ();

  // Install UDP server and client applications based on the configuration
  validator.InitializeUdpApplication ();

  // Run the simulation
  Simulator::Stop(Seconds(40));
  Simulator::Run ();
  Simulator::Destroy ();

  std::cout << "Simulation completed successfully." << std::endl;
  return 0;
}

} // namespace ns3

// Application entry point
int
main (int argc, char *argv[])
{
  return ns3::main (argc, argv);
}
