import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json

#sklearn helpers 
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split

from samples_tthml import *

seed=8
np.random.seed(seed)


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

def pred_ds(dfs,test_samp_size=0.33):    
    X = np.concatenate((dfs['ttH'],dfs['ttW']))
    sc = StandardScaler()
    X = sc.fit_transform(X)
    y = np.concatenate((np.ones(dfs['ttH'].shape[0]),np.zeros(dfs['ttW'].shape[0]))) # class lables                                                                       
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = test_samp_size)
    return X_train, X_test, y_train, y_test

def main():

    dfs=data_load(samples)
    X_train, X_test, y_train, y_test = pred_ds(dfs)


if __name__ == "__main__":
    main() 
