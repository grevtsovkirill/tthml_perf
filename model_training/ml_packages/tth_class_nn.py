import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json

from samples_tthml import *

def sel_vars(list_name="../VarList_30.json"):
    with open(list_name) as vardict:
        variablelist = json.load(vardict)[:]    
    return variablelist

def data_load(in_list, do_clean=True):
    df = {}
    if do_clean:
        var_list=sel_vars()

    for s in samples:
        print(s,'  ',samples[s]['filename'])
        df[s] = pd.read_csv(BASE+samples[s]['filename'])
        if do_clean:
            df[s] = df[s][var_list]
    return df
    
def main():

    dfs=data_load(samples)
    print(dfs['ttH'].head())
    
if __name__ == "__main__":
    main() 
