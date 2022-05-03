import time
import os
import animation

threshold = 45
th = threshold * (-1)

duration = 0.125    # The duration of time in seconds between each frame.
cycles = 10       # The number of cycles the animation plays through.

def animate():
    """Iterate through the frames, printing then clearing each one to create an animation."""
    count = 0
    while count < cycles:
        for frame in animation.frames:
            print '\033[1;34m' + frame + '\033[1;m'  # Print the frame in color blue.
            time.sleep(duration)
            print(os.system('clear'))
        count = count + 1

# animate()

while True:

    cnt = 0

    for i in row['corrected']:
        xx = i[0]
        yy = i[1]
        if (yy > th || xx > threshold || yy < th):
            cnt += 1
            if (cnt == 500000):
                animate()
                cnt = 0
        else:
            cnt = 0
