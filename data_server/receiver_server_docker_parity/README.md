# Docker 

To build from zero, the easiest way is copy this directory to your "server-side machine" and run the following commands

We tested under `Linux raspberrypi 5.15.32-v8+ aarch64` with docker version 20.10.14
## Build 

Assume you are under <wherever_you_put_this_directory>/receiver_server_docker

``` sh
# build the docker image
sudo docker build -t py_socket_server_parity .

# check your image list
sudo docker images
```



## Run the built image
```sh
# run the second socket server
sudo docker run -d -p 4001:4001 --name py_server_1 py_socket_server_parity

# check status
sudo docker ps
# copy the <CONTAINER ID> of your container
# check output of stdout
sudo docker logs --follow <CONTAINER ID>
```

you should expect something like: (`socket_client.py is triggered on the "client-side" after we start the container)
```
pi@raspberrypi:~/Workspace/receiver_server_docker_parity $ sudo docker logs --follow ded
[Server] Start server at:  2022-05-02 00:58:33.324147
[Server] Starting up on 0.0.0.0 port 4001
[Server] Waiting for a connection
[Server] Connection from ('192.168.50.93', 39239) start at:  2022-05-02 00:59:10.647033
number: 0000
content:(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  3.87, -3.71),(  4.04, -3.63),(  4.04, -3.63),(  4.04, -3.63),
parity: d1e5a73762f721a1a6e8b8dc2abb394a
number: 0001
content:(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),(  4.04, -3.56),
parity: 7fb70031de5f80ace6c0ff870937b725
number: 0002
content:( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),( -1.08,-44.17),
parity: e1214b325871ebaefe5640043d104e04
number: 0003
content:(-14.04,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),( -8.50,-44.85),
parity: d6e89dc55708bac26c4a7af7c24c7482
number: 0004
content:( -1.52,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),(  7.10,-43.62),
parity: f0daf84d056476ebc7d7e5604f8d7fd2
number: 0005
content:( 22.85,-43.11),( 22.85,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),( 24.65,-43.04),
```