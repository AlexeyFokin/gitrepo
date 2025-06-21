import numpy as np
import simpleaudio as sa
import serial


# get timesteps for each sample, T is note duration in seconds
sample_rate = 8000


ser = serial.Serial('COM9', 115200)
audio = np.array([])

for i in range(3*8000):
    try:
        line = ser.readline()
        line = line.decode('ascii')
        line = int(line.rstrip()) / 256
        audio = np.append(audio, line)
        #print(f'{line}')
    except:
        pass

print(audio.max(), audio.min())

audio = audio / 1000000


# # concatenate notes
# audio = np.hstack((A_note, Csh_note, E_note))
# # normalize to 16-bit range
audio *= 32767 # / np.max(np.abs(audio))
# # convert to 16-bit data
audio = audio.astype(np.int16)
#
# # start playback
play_obj = sa.play_buffer(audio, 1, 2, sample_rate)
#
# # wait for playback to finish before exiting
play_obj.wait_done()
