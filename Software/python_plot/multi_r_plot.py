import pandas
import statistics
import matplotlib
matplotlib.use('Qt5Agg')

import matplotlib.pyplot as plt
import numpy as np
def zero_bl(data):
    
    d= np.concatenate(data.values)

    a = statistics.mean(d[0:50])
    return d-a


widths = [8] * 32

# Leggi il file con le 32 colonne e senza riga di intestazione
data_0 = pandas.read_fwf('w_0.txt', header=None, widths=widths, colClasses="integer")



pandas.set_option('display.max_rows', None)



# Creiamo un subplot con 8 righe e 4 colonne (8x4) per i 32 canali
fig, axs = plt.subplots(8, 4, figsize=(12, 12))

# Iteriamo su tutti i canali e plottiamo ciascuno in un subplot separato
for i in range(32):
    row = i // 4
    col = i % 4
    axs[row, col].plot(data_0[i], color="blue")
    axs[row, col].set_title(f"Channel {i}")
    axs[row, col].grid(True)

# Opzionalmente, puoi aggiustare il layout e aggiungere un titolo alla figura
fig.tight_layout()
fig.suptitle("Digitizer 32 channels", fontsize=16)

# Visualizza il plot
plt.show()