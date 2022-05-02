import subprocess
import sqlite3
import pandas as pd
import random


def random_erasure(content):
    start_at = random.randint(0, 224 - 16)
    tmp = list(content)
    tmp[start_at:start_at+16] = '~'*16
    content = ''.join(tmp)
    return content


if __name__ == "__main__":
    # Content parity database
    db_parity = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/parity_0.sqlite")
    parity_df = pd.read_sql('SELECT * FROM test ', db_parity)
    db_parity.close()

    # Connect to content database
    db_0 = sqlite3.connect("/home/pi/Workspace/receiver_server_docker/content_0.sqlite")
    content_0_df = pd.read_sql("select * from experiments", db_0)
    db_0.close()
    
    t_df = parity_df.merge(content_0_df, on='num')
    print(parity_df)
    print(t_df)

    for index, row in t_df.iterrows():
        row['content'] = random_erasure(row['content'])

    print(t_df)

    for index, row in t_df.iterrows():
        print("{}".format(row['content']))

        res = subprocess.call(["/home/pi/Workspace/Reed-Solomon-Encoder-Decoder-/rs_decoder", 
        row['content'], row['parity']])



    

