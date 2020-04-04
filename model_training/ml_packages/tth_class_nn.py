import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json

#sklearn helpers: 
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split

#keras imports:
from keras.layers import Layer, Input, Dense, Dropout
from keras.models import Sequential, load_model

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

def create_model(my_learning_rate):
    dense_dim=len(sel_vars())
    model = Sequential()
    model.add(Dense(dense_dim, input_dim=dense_dim, activation='relu'))
    model.add(Dropout(rate=0.1, noise_shape=None, seed=None))
    model.add(Dense(50, activation='relu'))
    model.add(Dense(1, activation='sigmoid'))
    model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model

def main():

    dfs=data_load(samples)
    X_train, X_test, y_train, y_test = pred_ds(dfs)
    model = create_model(0.01)

if __name__ == "__main__":
    main() 
