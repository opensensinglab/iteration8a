from leanuvc import *
import time

#np.set_printoptions(linewidth=np.nan) #Don't add newlines!
np.set_printoptions(threshold='nan')	 #Output whole matrix

number_of_frames = 1  	#Collect this many frames
time_to_sleep = 2	#Wait this long in between

while True:
	for i in range(0,number_of_frames):
		rw  = GetData()
		rw1d = np.reshape(rw,-1)
		tempsF =ktof(rw1d)
		x_arrastr = np.char.mod('%.1f',tempsF)
		ss = " ".join(x_arrastr)
	#	ss = np.array2string(tempsF, separator = ' ')
#		out = np.array(["%.1f" % ktoc(w) for w in rw.reshape(rw.size)])
		out = np.array(["%.0f" % ktoc(w) for w in rw.reshape(rw.size)])
		out = out.reshape(rw.shape)
#		print out

		print ""
		for x in range(0, 60):
			for y in range(0, 80):
				value = out[x, y]
				print value + " ",
			print ""
				

	#print ss
	time.sleep(time_to_sleep)
