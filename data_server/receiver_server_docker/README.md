# Docker 

To build from zero, the easiest way is copy this directory to your "server-side machine" and run the following commands

We tested under `Linux raspberrypi 5.15.32-v8+ aarch64` with docker version 20.10.14
## Build 

Assume you are under <wherever_you_put_this_directory>/receiver_server_docker

``` sh
# build the docker image
sudo docker build -t py_socket_server .

# check your image list
sudo docker images
```



## Run the built image
```sh
# run the first socket server
sudo docker run -d -p 4000:4000 --name py_server_1 py_socket_server
# check status
sudo docker ps
# copy the <CONTAINER ID> of your container
# check output of stdout
sudo docker logs <CONTAINER ID>
```

you should expect something like: (`socket_client.py is triggered on the "client-side" after we start the container)
```
pi@raspberrypi:~/Workspace/receiver_server_docker $ sudo docker logs 88a1bfdbc7dc
starting up on 0.0.0.0 port 4000
waiting for a connection
connection from ('192.168.50.93', 51228)
received "This is the 0th message.  It will be repeated."
received "This is the 1th message.  It will be repeated."
received "This is the 2th message.  It will be repeated."
received "This is the 3th message.  It will be repeated."
received "This is the 4th message.  It will be repeated."
received "This is the 5th message.  It will be repeated."
received "This is the 6th message.  It will be repeated."
received "This is the 7th message.  It will be repeated."
received "This is the 8th message.  It will be repeated."
received "This is the 9th message.  It will be repeated."
received ""
Received 10 packets
no more data from ('192.168.50.93', 51228)
waiting for a connection
```