import socket

# Create a TCP/IP socket
sock = socket.create_connection(('192.168.4.1', 4000))

try:
    for i in range(0,10):
        # Send data
        message = "This is the {}th message.  It will be repeated.".format(i)
        print("sending \"{}\"".format(message))
        sock.sendall(str.encode(message))

        amount_received = 0
        amount_expected = len(message)
        
        data = sock.recv(64)
        amount_received += len(data)
        print('received "{}"'.format(data))

finally:
    print('closing socket')
    sock.close()