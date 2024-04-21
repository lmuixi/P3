import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import soundfile as sf

signal, fm = sf.read('rl002.wav') # Usamos señal rl002
t = np.arange(0, len(signal)) / fm 

t_ms = 30                 # 30 ms
l = int((fm * t_ms)/1e3)  # 'l' muestras

def autocorrelacion(vector):
    autocorrelation = np.correlate(vector, vector, mode = 'full')
    return autocorrelation[autocorrelation.size//2:]

   # Create a figure with a 2x2 grid of subplots
fig, axs = plt.subplots(2, 2, figsize=(10, 6))

   # Plot the complete signal in the top-left subplot
axs[0, 0].plot(t, signal)
axs[0, 0].set_title('Señal completa (rl002)', fontweight='bold')
axs[0, 0].set_xlabel('s', fontsize=10)
axs[0, 0].set_ylabel('Amplitud', fontsize=10)

   # Plot the 30ms signal segment in the top-right subplot
axs[0, 1].plot(t[fm:fm+l], signal[fm:fm+l])
axs[0, 1].set_title('Señal recortada (30ms)', fontweight='bold')
axs[0, 1].set_xlabel('Muestras', fontsize=10)
axs[0, 1].set_ylabel('Amplitud', fontsize=10)

   # Plot the autocorrelation in the bottom-left subplot
axs[1, 0].plot(t[:l]*1000, autocorrelacion(signal[fm:fm+l]))
axs[1, 0].set_title('Autocorrelación', fontweight='bold')
axs[1, 0].set_xlabel('n', fontsize=10)
axs[1, 0].set_ylabel('Amplitud', fontsize=10)

   # Remove the bottom-right subplot
fig.delaxes(axs[1, 1])

   # Adjust the spacing between the subplots
fig.subplots_adjust(hspace=0.4, wspace=0.3)

   # Display the figure
plt.show()