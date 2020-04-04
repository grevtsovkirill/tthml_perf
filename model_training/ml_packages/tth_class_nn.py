import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json

from samples_tthml import *

def data_load(in_list):
    for s in samples:
        print(samples[s]['filename'])

def main():

    dfs=data_load(samples)

if __name__ == "__main__":
    main() 
