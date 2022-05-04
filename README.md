# EC544_Project
A Neck Healthcare System Based on FRDM Sensor, designed with distribution storage system as the backend.

## How to run this project?

### Hardware prerequisite:
1. NXP FRDM-K64F development board 
2. FRDM-K64F-AGM04 Sensor Shield
3. Raspberry Pi 4 / 400 (and Micro SD card, power cable, etc)
4. Ethernet Cable (connect FRDM board to local network)
5. Usb cable (connect your FRDM board to the host computer)
6. Router

### Software prerequisites:
1. Mbed Studio (to build and write image to FRDM board)
2. ssh terminal (to communicate with the RPI)



### Steps
(these steps are tested with MacOS 12.3.1, Raspberry Pi OS Debian Version 11, 32-bit)
1. clone this repo to your host computer 
   1. use Mbed Studio to open directory `EC544_Project/sensor_data_transmit`
   2. copy the content of directory `EC544_Project/data_server` to your raspberry pi's directory `/home/pi/Workspace` (you can modify the source code if this directory is not available)

2. In Mbed studio, open `EC544_Project/sensor_data_transmit` as a project, then connect your FRDM-K64F to the host. 
   1. According to yor local network configuration, modify the content of file `EC544_Project/sensor_data_transmit/mbed_app.json`
   2. Select the right target board on the device section.
   3. Click on the "Run" button to build and write image to the board
   4. By default, once the image is downloaded to the board, it would start the process of establish ethernet and try to setup TCP Socket with the pre-defined server. Normally in this step you would get a error message, if you haven't setup the server yet, don't be panic, just ignore it. 
   5. You are all set on the client side for now
3. Now we turn to the Raspberry Pi, which is the server side
   1. In the terminal, cd to `/home/pi/Workspace`
   2. To start the normal server:
      1. cd to `/home/pi/Workspace/receiver_server_docker_parity`
      2. run python script with `python -u socket_server.py`
      3. or you can also run it in background with `python -u socket_server.py > server_result.txt &`
   3. To start the comparison server:
      1. cd to `/home/pi/Workspace/receiver_server_docker`
      2. run python script with `python -u socket_server.py`
      3. or you can also run it in background with `python -u socket_server.py > server_result.txt &`
   4. To test the function of our Reed-Solomon decoder, you have to compile the RSCODEC first, please follow the instructions in `/home/pi/Workspace/RSCODEC/README.md`.
    
      then you can cd to `/home/pi/Workspace/` and launch with `python -u rs_decoder_demo.py`

   5. To launch the real-time monitor service,
      1. cd to `/home/pi/Workspace`
      2. launch `python -u terminal_animation.py`


Enjoy the demo program!!

Demo Video can be found [here](https://www.youtube.com/watch?v=3-NFK4SuPOo)
     

## Development Notes:

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
- [x] Implement message queue 
- [x] Integrate sensor data module + codec module 
  - [x] + transmitting module 
- [x] update server side decoding module and server side service program
  - [x] dockerize the server side with dockerfile
  - [x] store the received message inside of the container (note that for this method, the lifetime of these "stored" contents would only match the lifetime of this container)
  - [ ] start multiple containers in one script
- [x] retreive data from server decoding method 
- [x] simulate neck movement in command line interface.