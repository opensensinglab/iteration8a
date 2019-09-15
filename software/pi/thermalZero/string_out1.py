from leanuvc import *
import time

#np.set_printoptions(linewidth=np.nan) #Don't add newlines!
np.set_printoptions(threshold='nan')	 #Output whole matrix

numFrames = 20  	# Collect this many frames
displayToConsole = False

# Keep track of time (to calculate framerate)
startTime = time.time()

for i in range(0, numFrames):
	# Get frame
	print "* Reading Data"
	rw  = GetData()
	print "* Data received"
#		rw1d = np.reshape(rw,-1)
#		tempsF =ktof(rw1d)
#		x_arrastr = np.char.mod('%.1f',tempsF)
#		ss = " ".join(x_arrastr)
	#	ss = np.array2string(tempsF, separator = ' ')
#		out = np.array(["%.1f" % ktoc(w) for w in rw.reshape(rw.size)])

	# Create a 2D array of strings representing the temperature at each pixel
	if (displayToConsole == True):
		print "* Formatting Data"
		out = np.array(["%.0f" % ktoc(w) for w in rw.reshape(rw.size)])
		out = out.reshape(rw.shape)
	
		# Display
		print ""
		print "Frame " + str(i)
		for x in range(0, 60):
			for y in range(0, 80):
				value = out[x, y]
				print value + " ",
			print ""
				

	#print ss
#	time.sleep(time_to_sleep)

# Calculate framerate
endTime = time.time()
print("Total runtime: " + str(endTime - startTime))
framerate = 1 / ((endTime - startTime) / numFrames)
print("Framerate: " + str(framerate) + " frames per second")

