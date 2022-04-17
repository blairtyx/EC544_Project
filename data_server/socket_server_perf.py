import socket
import datetime

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ("192.168.50.206", 4000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

counter = 0
auto_time = datetime.datetime.now()
print("Auto_time", auto_time)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from {}'.format(client_address))
        start_time = datetime.datetime.now()
        print("Now time is ", start_time)
        if datetime.datetime.now() - auto_time > datetime.timedelta(minutes=5):
            break
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(64)
            if(counter ==0):
                start_time = datetime.datetime.now()
            received_time = datetime.datetime.now() - start_time
            if data:
                # print('[{}] received "{}"'.format(received_time ,data))
                counter += 1
            else:
                print('no more data from', client_address)
                break
            
            if (received_time > datetime.timedelta(seconds=10)):
                print("Ten seconds transmitted {} packets".format(counter))
                counter = 0
                start_time = datetime.datetime.now()
            elif (counter % 1000 == 0):
                print("Received ", counter, "packets")
            if datetime.datetime.now() - auto_time > datetime.timedelta(minutes=3):
                break
        
    finally:
        # Clean up the connection
        connection.close()
        exit()