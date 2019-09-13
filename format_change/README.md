# Transform root ntuples to "industry-conventional" format

## Convert root to dataframe
`access_root.ipynb` - use uproot to convert root to `pandas` DataFrame.

Load signal/backgorund, check consistency of the content, plot variables for overlaid Signal and Background:

![Example](Plots/var.jpg)

## Convert to csv and HDF5
Use built in functions of `pandas` to suitable format.
 * root
     * ttH.root 7.7M 
     * ttW.root 9.7M
 * CSV:
     * ttH.csv  3.7M  
     * ttW.csv  3.7M  
 * [HDF5](https://www.hdfgroup.org/solutions/hdf5/):
     * ttW.h5  1.8M  
     * ttH.h5  1.8M  

&rarr; HDF5 4.3-5.4 times more efficient in terms of space
