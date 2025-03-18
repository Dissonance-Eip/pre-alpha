import matplotlib.pyplot as plt

# Read audio data from file
with open('audio_data.txt', 'r') as file:
    audio_data = [int(line.strip()) for line in file]

# Plot the audio data
plt.plot(audio_data)
plt.title("Audio Data Waveform")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.show()