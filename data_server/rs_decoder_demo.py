import subprocess
import sqlite3
import pandas as pd
import random


def random_erasure(content):
    start_at = random.randint(0, 14)
    tmp = list(content)
    
    tmp[start_at*16:(start_at+1)*16] = '~'*16
    content = ''.join(tmp)
    return content


if __name__ == "__main__":
    # Content parity database
    db_parity = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/parity_0.sqlite")
    parity_df = pd.read_sql('SELECT * FROM parity ORDER BY time DESC LIMIT 1', db_parity)
    db_parity.close()

    # Connect to content database
    ## First Half 
    db_0 = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/content_0.sqlite")
    for i in range(0,7):
        queryString = "select content, time from segment{} ORDER BY time DESC LIMIT 1".format(i)
        temp_df = pd.read_sql(queryString, db_0)
        parity_df = parity_df.merge(temp_df, on='time')
        if i != 0:
            parity_df['content'] = parity_df['content_x'] + parity_df['content_y']
            parity_df.drop(['content_x', 'content_y'], axis=1, inplace=True)
    db_0.close()

    ## Second Half
    db_1 = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/content_1.sqlite")
    for i in range(0,7):
        queryString = "select content, time from segment{} ORDER BY time DESC LIMIT 1".format(i)
        temp_df = pd.read_sql(queryString, db_1)
        parity_df = parity_df.merge(temp_df, on='time')
        parity_df['content'] = parity_df['content_x'] + parity_df['content_y']
        parity_df.drop(['content_x', 'content_y'], axis=1, inplace=True)
    db_1.close()

    # Connect to Comparison database
    db_com = sqlite3.connect("/home/pi/Workspace/receiver_server_docker/content_0.sqlite")
    com_df = pd.read_sql("select * from demo order by time desc limit 1", db_com)
    
    # simulate the condition if one segment is missing 
    for index, row in parity_df.iterrows():
        row['content'] = random_erasure(row['content'])

    # correct and store the corrected result in column
    parity_df['corrected'] = "0"
    for index, row in parity_df.iterrows():
        res = subprocess.Popen(["/home/pi/Workspace/RSCODEC/rs_decoder", 
                            row['content'], row['parity']], stdout=subprocess.PIPE)
        
        out = res.communicate()
        row['corrected'] = out[0].decode().strip('\n')

    # compare with the "origin" message
    com_df = com_df.merge(parity_df, on='num', suffixes=('_compare', '_merged'))
    com_df['check'] = com_df['content_compare'].eq(com_df['corrected'])

    for index, row in com_df.iterrows():
        print("Comparison String:     ", row['content_compare'])
        print("multidb-Merged String: ", row['content_merged'])
        print("RS_corrected String:   ", row['corrected'])
        print("Compare Result:        ", row['check'])

    

