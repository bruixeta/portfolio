import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn import metrics
import pandas as pd

M = 20 #Mida de la Matriu

#%% Definició de funcions
def EnergiaInd(Matriu, i, j):
    return -((Matriu[i,(j+1)%M] + Matriu[i,(j-1)%M] + Matriu[(i+1)%M,j] + Matriu[(i-1)%M,j]) * Matriu[i,j])/2

def EnergiaStep(Matriu, i, j):
    return (EnergiaInd(Matriu, i, j) + EnergiaInd(Matriu, i, (j+1)%M) + EnergiaInd(Matriu, i, (j-1)%M) + EnergiaInd(Matriu, (i+1)%M, j) + EnergiaInd(Matriu, (i-1)%M, j))

def Mean_Energy(Matriu):
    Energies = Matriu.copy()
    for i in range(M):
        for j in range(M):
            Energies[i,j] = EnergiaInd(Matriu,i,j)
    Promig = np.mean(Energies)
    return Promig

def Energia_Total(Matriu):
    suma = 0
    for i in range(M):
        for j in range(M):
            suma += EnergiaInd(Matriu,i,j)
    return suma/(2*M*M)

def Magnetitzacio(Matriu):
    magnetitza = np.abs(np.sum(Matriu))/(M*M)
    return magnetitza

def Metropolis(Matriu, T):
    i = np.random.randint(0,M)
    j = np.random.randint(0,M)
    Energia0 = EnergiaStep(Matriu,i,j)
    Matriu2 = Matriu.copy()
    Matriu2[i,j] = -(1.0)*Matriu[i,j] 
    Energia1 = EnergiaStep(Matriu2,i,j)
    DeltaE = (Energia1-Energia0)
    
    if DeltaE > 0: 
        prob = np.exp(-DeltaE/T)
        random = np.random.random()
        if random < prob:
            Matriu[i,j] = Matriu2[i,j]
    else: 
        Matriu[i,j] = Matriu2[i,j]
    
    return Matriu

def Nequilibri(Temperatures):
    enes = []
    for t in Temperatures:
        Matriu = np.matrix(np.random.choice((-1,1), (M,M)))
        count = 0
        ena = 0
        Energies=[]
        mitjana0=0
        for n in range(N):
            ena=n
            Matriu = Metropolis(Matriu, t)
            Energies.append(Energia_Total(Matriu))
            if((n)%10000==0):
                mitjana1=np.mean(Energies)
                DeltaMitjanes=mitjana1-mitjana0
                if abs(DeltaMitjanes) < 0.01:
                    count += 1
                else:
                    Energies=[]
                    count = 0 
                mitjana0=mitjana1
            if count==5:
                ena-=5*10000
                break
        enes.append(ena)
    return enes

def MB(t, N):
    Matriu = Equilibri_Provisional(t)
    
    nn = 0
    Energia_Individual = []
    
    while nn < N:
        Matriu = Metropolis(Matriu,t)
        for i in range(M):
            for j in range(M):
                Energia_Individual.append(EnergiaInd(Matriu, i, j))

        nn += 1
    
    return Energia_Individual

def Equilibri_Provisional(t):
    
    N_gran= 150000
    N_petit = 40000
    N_extrem = 1000000
    
    Mitjana_Magnetitzacions = []
    Mitjana_Energies = []
    
    Matriu = np.matrix(np.random.choice((-1,1), (M,M)))
    
    if t < 1.6: 
        N_equilibri = N_petit
    if ((t >= 1.6) and (t <= 2)): 
        N_equilibri = N_gran
    if ((t < 3) and (t > 2)): 
        N_equilibri = N_extrem
    if t >= 3: 
        N_equilibri = N_petit
    
    
    n = 0
    
    while n<N_equilibri:
        Matriu = Metropolis(Matriu,t)
        n += 1
    
    return Matriu
#%% Magnet vs N (i Energia vs N)
N = 500000 #Nombre de passos
Matriu = np.matrix(np.random.choice((-1,1), (M,M)))
Matriu_Inicial = Matriu.copy()
T = 1.
Energia = []
Magnet= []
for n in range(N):
    Matriu = Metropolis(Matriu, T)
    Magnet.append(Magnetitzacio(Matriu))
    Energia.append(Energia_Total(Matriu))
Matriu_Final = Matriu.copy()

plt.plot(range(N), Magnet)
plt.xlabel("Nombre de passos")
plt.ylabel("Magnetització")
plt.ylim([-0.05, 1.05])

plt.plot(range(N), Energia)
plt.xlabel("Nombre de passos")
plt.ylabel("Energia mitjana")
plt.ylim([-1.05, 0.05])

plt.figure() #plot de les configuracions inicial i final
plt.subplot(211)
plt.subplots_adjust(bottom=0.1, right=1, top=0.9)
plt.imshow(Matriu_Inicial, vmin = -1, vmax = 1, cmap = 'viridis')
plt.axis('off')

plt.subplot(212)
plt.imshow(Matriu_Final,vmin = -1, vmax = 1, cmap = 'viridis')
plt.axis('off')
plt.show()


#%%
N_gran= 150000
N_petit = 40000
N_extrem = 1000000
N = 50000

Mitjana_Magnetitzacions = []
Mitjana_Energies = []
Cv = []
Susceptibilitat = []

Temperatures = np.linspace(0.5,4.0,100)

for t in Temperatures:
    Magnet = []
    Energia = []
    Matriu = np.matrix(np.random.choice((-1,1), (M,M)))
    
    if t < 1.6: 
        N_equiliri = N_petit
    if ((t >= 1.6) and (t <= 2)): 
        N_equilibri = N_gran
    if ((t < 3) and (t > 2)): 
        N_equilibri = N_extrem
    if t >= 3: 
        N_equilibri = N_petit
   
    
    n = 0
    
    while n<N_equilibri:
        Matriu = Metropolis(Matriu,t)
        n += 1
        
    nn = 0
    
    while nn < N:
        Matriu = Metropolis(Matriu,t)
        Magnet.append(Magnetitzacio(Matriu))
        Energia.append(Energia_Total(Matriu))
        nn += 1
    
    Mitjana_Magnetitzacions.append(np.mean(Magnet))
    Mitjana_Energies.append(np.mean(Energia))
    Susceptibilitat.append(np.std(Magnet))
    Cv.append(np.std(Energia))

#%% Magnetització, Energia, Calor específica i susceptibilitat magnètica vs T
MvsT = Mitjana_Magnetitzacions.copy()
EvsT = Mitjana_Energies.copy()
XvsT = Susceptibilitat.copy()
CvvsT = Cv.copy()

plt.scatter(Temperatures, MvsT, s = 25., marker = 'x')
plt.xlabel("Temperatura")
plt.ylabel("Magnetització")

plt.scatter(Temperatures, EvsT, s = 25, marker = 'x')
plt.xlabel("Temperatura")
plt.ylabel("Energies")

plt.scatter(Temperatures, XvsT, s = 25, marker = 'x')
plt.xlabel("Temperatura")
plt.ylabel("Susceptibilitat")

plt.scatter(Temperatures, CvvsT, s = 25, marker = 'x')
plt.xlabel("Temperatura")
plt.ylabel("Calor específica")
#%% Càlcul de l'exponent crític
T = np.linspace(0.5,4.0,100)
Magnet_Trampa = MvsT.copy()

df = pd.DataFrame()
df['M'] = Magnet_Trampa
df['T'] = Temperatures

df = df[df['T'] > 1.8]
df = df[df['T'] < 2.3]

Temp = df['T']
Magnet = df['M']

log_T = np.array(np.log(np.abs(Temp-2.3)))
log_M = np.log(Magnet)
plt.scatter(log_T, log_M)

reg = LinearRegression().fit(log_T.reshape(-1,1), log_M)
print(reg.coef_) #pendent de la recta
print(metrics.mean_squared_error(log_T, log_M)) #error 
print(reg.score(log_T, log_M)) #coeficient de regressió
#%%
x = np.linspace(-3.5, 0, 14)
y = reg.coef_*x + reg.intercept_
plt.plot(x,y,  color = 'red')

plt.scatter(log_T, log_M)
plt.xlabel('log(T-Tc)')
plt.ylabel('log(M)')


#%% Maxwell-Boltzmann
T = 10
N = 100

T_1 = MB(T, N)
plt.hist(T_1,[-2,-1,0,1,2,3] ,density = True, color = 'paleturquoise', edgecolor = 'black',align='left',zorder=1)
plt.xlabel('Energia')
plt.ylim(0,1)

x = np.linspace(-2,2,5)
y = []
coef = [2,4,8,4,2]
A = (2*np.exp(2./T)+4*np.exp(1./T)+8+4*np.exp(-1./T)+2*np.exp(-2./T))
for i in range(len(x)):
    y.append(np.exp(-x[i]/T)*coef[i]/A)


plt.scatter(x, y, color = 'red',zorder=2)




