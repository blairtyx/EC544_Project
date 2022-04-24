import socket
import time

# Create a TCP/IP socket
sock = socket.create_connection(('192.168.50.207', 4000))

try:
    for i in range(0,10):
        # Send data
        message = "This is the {}th message.  It will be repeated.".format(i)
        print("sending \"{}\"".format(message))
        sock.sendall(str.encode(message))
        time.sleep(5)
finally:
    print('closing socket')
    sock.close()