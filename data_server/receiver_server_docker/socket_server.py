import socket
import datetime
import sqlite3


if __name__ == "__main__":
    counter = 0
    auto_time = datetime.datetime.now()
    print("[Server] Start server at: ", auto_time)


    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ("0.0.0.0", 4000)
    print('starting up on {} port {}'.format(*server_address))
    sock.bind(server_address)
    sock.listen(1)

    # Content databases
    db_0 = sqlite3.connect("content_0.sqlite")
    cur_0 = db_0.cursor()
    cur_0.execute("CREATE TABLE IF NOT EXISTS experiments (num TEXT, content TEXT)")
    db_0.commit()


    while True:
        # Wait for a connection
        print('[Server] Waiting for a connection')
        connection, client_address = sock.accept()
        try:
            start_time = datetime.datetime.now()
            print('[Server] Connection from', client_address, 'start at: ', start_time)
            # Receive data 
            while True:
                data = connection.recv(256)
                if data:
                    print("number: {}".format(data[0:4].decode()))
                    print('content:{}'.format(data[4:228].decode()))
                    cur_0.execute("INSERT INTO experiments (num, content) values (\"{}\", \"{}\")".format(data[0:4].decode(), data[4:228].decode()))
                    db_0.commit()
                    counter += 1
                else:
                    print("Received {} packets".format(counter))
                    print('no more data from', client_address)
                    counter = 0
                    break
                
        finally:
            # Clean up the connection
            connection.close()
            db_0.close()
