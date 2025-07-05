# NS-3 Quality of Service (QoS) Simulation


## Developer
### Shreyas Yadav


## Project Overview

This project implements a Quality of Service (QoS) simulation using NS-3, featuring two scheduling algorithms:

1. **Strict Priority Queuing (SPQ)**: Always serves packets from the highest priority queue that has packets. Lower priority queues are only served when all higher priority queues are empty.

2. **Deficit Round Robin (DRR)**: Distributes bandwidth across multiple traffic classes according to their weight. Each traffic class receives service proportional to its configured weight.

The simulation sets up a client-router-server topology to demonstrate packet scheduling behavior, with configurable traffic classes and filtering capabilities.

## Project Structure

The project consists of the following components:

- **DiffServ**: Base class for Differentiated Services QoS mechanisms
- **SPQ**: Implementation of Strict Priority Queuing
- **DRR**: Implementation of Deficit Round Robin
- **TrafficClass**: Represents a queue with specific QoS parameters
- **Filter**: Classifies packets into appropriate traffic classes
- **Validation**: Handles configuration parsing and simulation setup

## Building and Running the Simulation

### Prerequisites

- NS-3 (version 3.36 or later)
- nlohmann/json library (for parsing configuration files)

### Installation

1. Place all source files in the `scratch/final-project/` directory of your NS-3 installation.

2. Make sure the nlohmann/json library is available in your NS-3 installation. If not, you may need to install it separately. 
    - Use "sudo apt install nlohmann-json3-dev" to install the package


### Running the Simulation

The simulation can be run with either SPQ or DRR configuration: 

#### Using SPQ (Strict Priority Queuing)

```bash
./ns3 run scratch/final-project/driver.cc -- scratch/final-project/spq_config.json
```

#### Using DRR (Deficit Round Robin)

```bash
./ns3 run scratch/final-project/driver.cc -- scratch/final-project/drr_config.json
```

### Configuration Files

The simulation is configured using JSON files:

- `spq_config.json`: Configuration for Strict Priority Queuing
- `drr_config.json`: Configuration for Deficit Round Robin

#### SPQ Configuration Example

```json
{
    "name":"spq",
    "queues": [
        {
            "no": 1,
            "Default": true,
            "MaxPackets": 1000,
            "Priority": 0,
            "DestPort": 10000,
            "SrcPort": 9000,
            "protocol": "",
            "mask": "",
            "DestIp": "",
            "SourceIp": ""
        },
        {
            "no": 2,
            "Default": false,
            "MaxPackets": 5000,
            "Priority": 1,
            "DestPort": 10001,
            "SrcPort": 9001,
            "protocol": "",
            "mask": "",
            "DestIp": "",
            "SourceIp": ""
        }
    ]
}
```

#### DRR Configuration Example

```json
{
    "name": "drr",
    "queues": [
        {
            "no": 1,
            "MaxPackets": 3000,
            "Weight": 10,
            "DestPort": 9000,
            "SrcPort": 9001,
            "protocol": "",
            "mask": "",
            "DestIp": "",
            "SourceIp": ""
        },
        {
            "no": 2,
            "MaxPackets": 3000,
            "Weight": 20,
            "DestPort": 10000,
            "SrcPort": 10001,
            "protocol": "",
            "mask": "",
            "DestIp": "",
            "SourceIp": ""
        }
    ]
}
```

### Output

The simulation generates packet capture (PCAP) files that can be analyzed with tools like Wireshark:

- `Pre_SPQ-1-0.pcap`: Packets before SPQ processing
- `Post_SPQ-1-1.pcap`: Packets after SPQ processing
- `Pre_DRR-1-0.pcap`: Packets before DRR processing
- `Post_DRR-1-1.pcap`: Packets after DRR processing




# Analyzing PCAP Files with Wireshark IO Graphs

## Setup

1. Install Wireshark from [wireshark.org](https://www.wireshark.org/download.html) if you haven't already
2. Open Wireshark and load the PCAP file by going to File → Open

## Viewing SPQ Traffic with IO Graph

1. Open your SPQ PCAP file in Wireshark
2. Go to Statistics → I/O Graph
3. In the IO Graph window, configure the following:
   - Set the desired time interval (e.g., 1 second) in the "Interval" field
   - Check "Draw as Line" for better visualization

4. Add filters for the first flow:
   - Click on the first empty filter row
   - Type: `udp.dstport == 10000` in the "Display filter" field
   - Give it a name (e.g., "SPQ Flow 1") in the "Name" field
   - Select a color (e.g., red) for this flow

5. Add filters for the second flow:
   - Click on the next empty filter row
   - Type: `udp.dstport == 10001` in the "Display filter" field
   - Give it a name (e.g., "SPQ Flow 2") in the "Name" field
   - Select a different color (e.g., blue) for this flow

6. You can adjust the Y-axis units as needed (packets/tick, bytes/tick, etc.)



## Viewing DRR Traffic with IO Graph

1. Open your DRR PCAP file in Wireshark
2. Go to Statistics → I/O Graph
3. Configure the graph with the same time interval settings as above

4. Add filters for the first flow:
   - Click on the first empty filter row
   - Type: `udp.dstport == 9000` in the "Display filter" field
   - Give it a name (e.g., "DRR Flow 1") in the "Name" field
   - Select a color (e.g., green) for this flow

5. Add filters for the second flow:
   - Click on the next empty filter row
   - Type: `udp.dstport == 10000` in the "Display filter" field
   - Give it a name (e.g., "DRR Flow 2") in the "Name" field
   - Select a different color (e.g., purple) for this flow

6. Add filters for the second flow:
   - Click on the next empty filter row
   - Type: `udp.dstport == 11000` in the "Display filter" field
   - Give it a name (e.g., "DRR Flow 2") in the "Name" field
   - Select a different color (e.g., yellow) for this flow

7. Click "Save" to preserve the graph configuration or "Copy" to copy the graph image

