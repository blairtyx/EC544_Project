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
sudo docker run -d -p 4000:4000 --name py_server_0 py_socket_server

# check status
sudo docker ps
# copy the <CONTAINER ID> of your container
# check output of stdout
sudo docker logs --follow <CONTAINER ID>
```

you should expect something like: (`socket_client.py is triggered on the "client-side" after we start the container)
```
pi@raspberrypi:~/Workspace/receiver_server_docker $ sudo docker logs --follow 1601
starting up on 0.0.0.0 port 4000
waiting for a connection
connection from ('192.168.50.93', 39987)
index: b'0000'
msg:   b'(  6.26, -7.60),(  6.26, -7.60),(  6.26, -7.60),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),(  6.00, -7.82),'
index: b'0001'
msg:   b'(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),(  6.33, -7.76),'
index: b'0002'
msg:   b'(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),(  6.28, -7.87),'
index: b'0003'
msg:   b'(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),(  5.12, -6.86),'
index: b'0004'
msg:   b'( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),( -4.04, 16.81),'
index: b'0005'
msg:   b'(  1.87, 18.48),(  1.87, 18.48),(  1.87, 18.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),(  0.73, 16.48),'
```