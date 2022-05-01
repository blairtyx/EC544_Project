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
connection from ('192.168.50.210', 63428)
index: b'0000'
msg:   b'(  5.82, -2.55),(  5.82, -2.55),(  5.82, -2.55),(  5.82, -2.55),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),(  5.71, -2.68),'
index: b'0001'
msg:   b'(  6.02, -2.46),(  6.02, -2.46),(  6.02, -2.46),(  6.02, -2.46),(  6.02, -2.46),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),(  6.00, -2.59),'
index: b'0002'
msg:   b'(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),(  6.06, -2.53),'
index: b'0003'
msg:   b'(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),(  6.06, -2.57),'
index: b'0004'
msg:   b'( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),( 19.53,-20.87),'
```