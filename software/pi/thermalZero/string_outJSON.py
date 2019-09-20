from __future__ import print_function
from leanuvc import *
import numpy as np
import time

def displayFrameJSON(frame):
        idx = 0;
#       print("data:[", end="");
	sizeX = frame.shape[0]
	sizeY = frame.shape[1]
        print("[", end="");
        for x in range(0, sizeX):
                print("[", end="")
                for y in range(0, sizeY):
			value = "%.0f" % ktoc(frame[x][y])
                        if (y < sizeY-1):
                                print(value + ", ", end="")
                        else:
                                print(value + "]", end="")
        
                        idx += 1
                if (x < sizeX-1):
                        print(", ", end="")
        print("]", end="")


#np.set_printoptions(linewidth=np.nan) #Don't add newlines!
np.set_printoptions(threshold='nan')	 #Output whole matrix

numFrames = 20  	# Collect this many frames

# Keep track of time (to calculate framerate)
startTime = time.time()

# Display start-of-data marker
print("*")

for i in range(0, numFrames):
	# Get frame
	rw  = GetData()

	# Display frame as JSON to console
	displayFrameJSON(rw);

	# Display end-of-frame marker
	print("")


