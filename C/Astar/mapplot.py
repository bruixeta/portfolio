#!/usr/bin/python3

# use as:
# $ python3 mapplot.py OutputExample.txt
# with matplotlib version >3.2.2 you can apply the solution from the following link:
# https://test.ocom.vn/?url=github.com/carlinmack/mplexporter/commit/739793fdda66809df80bb6b2d19c0605544f8292

import json
import os
import matplotlib.pyplot as plt
import numpy as np
import sys
import mplleaflet

# fitxer d'entrada
in_file = sys.argv[1]

# nom del fitxer de sortida: el mateix que l'entrada, canviant extensio per html
mapfile = in_file.split('.')[-2]+".html"

# convertim les dades del fitxer en un array del numpy
latitud  = []
longitud = []

with open(in_file, 'r') as f:
    for line in f:
        if line.startswith("#"): # Ignorem les linies que comencen per #
            continue
        fields = line.split('|')
        for field in fields:
            field.strip();
        latitud.append(float(fields[1]))
        longitud.append(float(fields[2]))

xy = np.array([[longitud[i],latitud[i]] for i in range(len(latitud))])

# Dibuixem el cami amb punts vermells connectats per linies blaves
fig = plt.figure()
plt.plot(xy[:,0], xy[:,1], 'r.')
plt.plot(xy[:,0], xy[:,1], 'b')

# Creem el mapa i el guardem amb el nom guardat a mapfile
mplleaflet.show(path=mapfile)
