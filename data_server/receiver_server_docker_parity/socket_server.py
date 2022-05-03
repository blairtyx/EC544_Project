from multiprocessing.dummy.connection import Connection
import socket
import datetime
import sqlite3


def gen_7_tables(db: sqlite3.Connection):
    cur = db.cursor()
    for i in range(0,7):
        cur.execute("CREATE TABLE IF NOT EXISTS segment{} (num TEXT, content TEXT, time TIMESTAMP)".format(i))
    db.commit()
    
def insert_data(db_0: sqlite3.Connection, db_1: sqlite3.Connection, data, time: datetime.datetime):
    cur_0 = db_0.cursor()
    cur_1 = db_1.cursor()
    number = data[0:4].decode()
    db_0_content = data[4:116].decode()
    db_1_content = data[116:228].decode()
    
    # put the first half to db_0 
    for i in range(0,7):
        cur_0.execute("INSERT INTO segment{} (num, content, time) values (\"{}\", \"{}\", \"{}\")".format(
                        i, number, db_0_content[i*16: (i+1)*16], time))
        cur_1.execute("INSERT INTO segment{} (num, content, time) values (\"{}\", \"{}\", \"{}\")".format(
                        i, number ,db_1_content[i*16: (i+1)*16], time))
    db_0.commit()
    db_1.commit()

    

if __name__ == "__main__":
    # init variables
    counter = 0
    auto_time = datetime.datetime.now()
    print("[Server] Start server at: ", auto_time)


    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ("0.0.0.0", 4001)
    print('[Server] Starting up on {} port {}'.format(*server_address))
    sock.bind(server_address)
    sock.listen(1)

    # Content databases
    db_0 = sqlite3.connect("content_0.sqlite", detect_types=sqlite3.PARSE_DECLTYPES | sqlite3.PARSE_COLNAMES)
    gen_7_tables(db_0)
    db_1 = sqlite3.connect("content_1.sqlite", detect_types=sqlite3.PARSE_DECLTYPES | sqlite3.PARSE_COLNAMES)
    gen_7_tables(db_1)
    
    # Parity database
    db_parity = sqlite3.connect("parity_0.sqlite", detect_types=sqlite3.PARSE_DECLTYPES | sqlite3.PARSE_COLNAMES)
    cur_p = db_parity.cursor()
    cur_p.execute("CREATE TABLE IF NOT EXISTS parity (num TEXT, parity TEXT, time TIMESTAMP)")
    db_parity.commit()

    # Connection
    while True:
        print('[Server] Waiting for a connection')
        connection, client_address = sock.accept()
        try:
            start_time = datetime.datetime.now()
            print('[Server] Connection from', client_address, 'start at: ', start_time)
            # Receive data 
            while True:
                data = connection.recv(256)
                currentDateTime = datetime.datetime.now()
                if data:
                    print("number: {}".format(data[0:4].decode()))
                    print('content:{}'.format(data[4:228].decode()))
                    print('parity: {}'.format(data[229:245].hex())) # one byte of terminal indicator
                    insert_data(db_0, db_1, data, currentDateTime)
                    cur_p.execute("INSERT INTO parity (num, parity, time) values (\"{}\", \"{}\", \"{}\")".format(
                                    data[0:4].decode(), data[229:245].hex(), currentDateTime))
                    db_parity.commit()
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
            db_parity.close()
            