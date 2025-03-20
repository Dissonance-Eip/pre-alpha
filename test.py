import matplotlib.pyplot as plt
import numpy as np

# Read audio data from binary file
with open('audio_data.bin', 'rb') as file:
    audio_data = np.fromfile(file, dtype=np.int16)

# Plot the audio data
plt.plot(audio_data)
plt.title("Audio Data Waveform")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.show()