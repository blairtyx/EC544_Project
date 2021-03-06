import socket

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ("192.168.4.1", 4000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

counter = 0

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(64)
            print('received "{}"'.format(data.decode()))
            if data:
                print('sending data back to the client')
                connection.sendall(str.encode("Im the server, cheers * {}".format(counter)))
                counter += 1
            else:
                print("Received {} packets".format(counter))
                print('no more data from', client_address)
                counter = 0
                break
            
    finally:
        # Clean up the connection
        connection.close()