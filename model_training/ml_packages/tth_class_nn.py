import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json

from samples_tthml import *

def data_load(in_list):
    df = {}
    for s in samples:
        print(s,'  ',samples[s]['filename'])
        df[s] = pd.read_csv(BASE+samples[s]['filename'])
    return df
    
def main():

    dfs=data_load(samples)
    print(dfs['ttH'].head())
    
if __name__ == "__main__":
    main() 
