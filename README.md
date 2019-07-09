## Performance of model training for ttH-Multilepton analysis (2lSS)

### Prepare samples:
#### Skimm root ntuples
Follow instructions from ```skim```:
Output is lightweight root files with weights and input variables, could be found at ```Files/skimmed```.
```ttH``` - signal, ```ttW``` - background.


#### Convert root to flat file
TBD


### TMVA :
#### Reproduce BDT with TMVA
Check nominal performance of the training with "fixed"-list of training variables used in current analysis.

TBD: optimization of the selection of the input variables to the model.

#### NN ?


### Industry tools:
#### scikit-learn DT
BDT with ada-boost?
