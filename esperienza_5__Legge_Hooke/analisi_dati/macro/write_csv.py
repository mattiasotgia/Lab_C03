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

L = np.array([38.0, 74.0, 90.0, 112.0, 144.0, 55.0, 128.0])/1000
err_L = np.ones(7)/(1000*np.sqrt(3))

T2 = T**2
err_T2 = 2*T*err_T

OUTPUT_FILES = [
    'p_oscillazioni_computed',
    'l_elong_computed',
]

import csv

with open(os.path.join(MISC_PATH, OUTPUT_FILES[0] + '.csv'), 'w') as output_dyn:
    writer_dyn = csv.writer(output_dyn, delimiter=',')
    writer_dyn.writerow(['M', 'err_M', 'T', 'err_T', 'T2', 'err_T2'])
    for i in range(0, len(T)):
        writer_dyn.writerow([M[i], err_M[i], T[i], err_T[i], T2[i], err_T2[i]])

with open(os.path.join(MISC_PATH, OUTPUT_FILES[1] + '.csv'), 'w') as output_sts:
    writer_sts = csv.writer(output_sts, delimiter=',')
    writer_sts.writerow(['M', 'err_M', 'L', 'err_L'])
    for i in range(0, len(M)):
        writer_sts.writerow([M[i], err_M[i], L[i], err_L[i]])