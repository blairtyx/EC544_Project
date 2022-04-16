import socket
import sys
import datetime

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ("192.168.3.1", 4000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
            print('[%s] received "%s"', str(datetime.datetime.now()) ,data)
            if data:
                pass 
                # print('sending data back to the client')
                # connection.sendall("Im the server, cheers")
            else:
                print('no more data from', client_address)
                break
            
    finally:
        # Clean up the connection
        connection.close()