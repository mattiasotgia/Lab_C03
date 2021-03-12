import os
import numpy as np
import pandas as pd


ALL_FILES = [
    'p_oscillazioni_NNx10.csv'
]
BASE_PATH = os.getcwd()
DATA_PATH = os.path.join(os.getcwd(), '../../dati')
MISC_PATH = os.path.join(os.getcwd(), '../../misc')

data_csv = pd.read_csv(os.path.join(DATA_PATH, ALL_FILES[0]), sep=',')

T = np.array(list((data_csv[line]/10).mean() for line in data_csv))
err_T = np.array(list((data_csv[line]/10).std()/np.sqrt(data_csv[line].size) for line in data_csv))

M = np.array([261.499, 535.07275, 650.44700, 796.66900, 1035.6340, 393.09100, 916.85700])/1000
err_M = np.array([0.004, 0.008, 0.012, 0.012, 0.016, 0.008, 0.016])/(1000*np.sqrt(3))

T2 = T**2
err_T2 = 2*T*err_T

OUTPUT_FILES = [
    'p_oscillazioni_computed'
]

import csv

with open(os.path.join(MISC_PATH, OUTPUT_FILES[0] + '.csv'), 'w') as output:
    writer = csv.writer(output, delimiter=',')
    writer.writerow(['M', 'err_M', 'T', 'err_T', 'T2', 'err_T2'])
    for i in range(0, len(T)):
        writer.writerow([M[i], err_M[i], T[i], err_T[i], T2[i], err_T2[i]])