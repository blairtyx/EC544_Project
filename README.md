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
  
FRDM-K64F tracing result:
```plaintext
[INFO][Main]: Start Reed-Soloman Test
[INFO][Main]: NPAR: 16, ML: 128
[INFO][Main]: Original data is:"536806816"

[INFO][Main]: Encoded data is: "536812045"

[INFO][Main]: with erasures: "N~r~o~s~y~I~l~a~e~ ~h~ ~w~n~d~c~s aboard the revolving platform. This is mainly the suppliment info to rep12123"

[INFO][Main]: End of demo
```

## Next Step
- [] Try message queue and integrate sensor data module + codec module + transmitting module 
- [] update server side decoding module and server side service daemon
- [] retreive data from server decoding method and simulate neck movement in command line interface.