import time
import os
import animation
import subprocess
import sqlite3
import pandas as pd
import ast

threshold = 45
th = threshold * (-1)

duration = 0.125    # The duration of time in seconds between each frame.
cycles = 10       # The number of cycles the animation plays through.



def animate():
    """Iterate through the frames, printing then clearing each one to create an animation."""
    print(os.system('clear'))
    for frame in animation.frames:
        print('\033[1;34m' ,  frame,  '\033[1;m')  # Print the frame in color blue.
        time.sleep(duration)
        print(os.system('clear'))

def good_postion():
    print(os.system('clear'))
    print(animation.frame_good)
    time.sleep(1)
    print(os.system('clear'))


if __name__ == "__main__":
    # Connect to databases
    db_parity = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/parity_0.sqlite")
    db_0 = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/content_0.sqlite")
    db_1 = sqlite3.connect("/home/pi/Workspace/receiver_server_docker_parity/content_1.sqlite")


    while(1):
        ## parity bits
        parity_df = pd.read_sql('SELECT * FROM parity ORDER BY time DESC LIMIT 1', db_parity)
        ## First Half Content
        for i in range(0,7):
            queryString = "select content, time from segment{} ORDER BY time DESC LIMIT 1".format(i)
            temp_df = pd.read_sql(queryString, db_0)
            parity_df = parity_df.merge(temp_df, on='time')
            if i != 0:
                parity_df['content'] = parity_df['content_x'] + parity_df['content_y']
                parity_df.drop(['content_x', 'content_y'], axis=1, inplace=True)
        ## Second Half Content
        for i in range(0,7):
            queryString = "select content, time from segment{} ORDER BY time DESC LIMIT 1".format(i)
            temp_df = pd.read_sql(queryString, db_1)
            parity_df = parity_df.merge(temp_df, on='time')
            parity_df['content'] = parity_df['content_x'] + parity_df['content_y']
            parity_df.drop(['content_x', 'content_y'], axis=1, inplace=True)

        # correct and store the corrected result in column
        parity_df['corrected'] = "0"
        for index, row in parity_df.iterrows():
            res = subprocess.Popen(["/home/pi/Workspace/RSCODEC/rs_decoder", 
                                row['content'], row['parity']], stdout=subprocess.PIPE)
            out = res.communicate()
            row['corrected'] = out[0].decode().strip('\n')
        
        
        cnt = 0
        for index, row in parity_df.iterrows():
            xypair_tuple = ast.literal_eval(row['corrected'])
            for xypair in xypair_tuple: 
                xx = xypair[0]
                yy = xypair[1]
                if (yy > th or xx >threshold or xx < th):
                    cnt += 1
            
        if (cnt > 10):
            animate()
        else:
            good_postion()
