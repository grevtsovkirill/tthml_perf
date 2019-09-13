## Performance of model training for ttH-Multilepton analysis (2lSS)

### Prepare samples:
#### Skimm root ntuples
Follow instructions from ```skim```:
Output is lightweight root files with weights and input variables, could be found at ```Files/skimmed```.
```ttH``` - signal, ```ttW``` - background.


#### Convert root to flat file
TBD


### Model training :
`model_training` - contains various sets of training for ttH vs ttW.

 * `standard_tmva` - nominal training using C++:
     * Basic functional - read trees, train (not include application)
 * `tmva_python` - python based use of TMVA, [`tthml_TMVAtraining_python.ipynb`](https://github.com/grevtsovkirill/tthml_perf/blob/master/model_training/tmva_python/tthml_TMVAtraining_python.ipynb):
     * Basic functional
         * Reproduce "nominal"
     * Introduce new features: 
         * Cross Validation
         * Plot combined ROC curves
         * Test NN implementation
 * Use industry-conventional tools:

#### Reproduce BDT with TMVA
Check nominal performance of the training with "fixed"-list of training variables used in current analysis.


#### T

TBD: optimization of the selection of the input variables to the model.

#### NN ?


### Industry tools:
#### scikit-learn DT
BDT with ada-boost?
