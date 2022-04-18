# EC544_Project
A Data Encrypted Neck Healthcare System Based on FRDM Sensor


## Data TX/RX 

### Server Side
Currently running python script on Raspberry Pi 400, with WIFI connection to router(192.168.50.1)

### Client Side
Means the FRDM-K64F side
- Enabled Ethernet Interface
- Enabled TCP Socket

### Dev Notes
- Tested Static IP TCP Socket Data Transmition between (2-way) on FRDM-K64F and RPI. 
  - Transmit performance: ~1000 packet/s, 128 byte/packet
- Create dedicated function based on current implementation. 

## Codec
- Test the basic Reed-Soloman Code operation. 

