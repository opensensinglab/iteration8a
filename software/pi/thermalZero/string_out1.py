from leanuvc import *
import time

#np.set_printoptions(linewidth=np.nan) #Don't add newlines!
np.set_printoptions(threshold='nan')	 #Output whole matrix

number_of_frames = 3  	# Collect this many frames

for i in range(0, number_of_frames):
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
	print "Formatting Data"
	out = np.array(["%.0f" % ktoc(w) for w in rw.reshape(rw.size)])
	out = out.reshape(rw.shape)
#		print out
	
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
