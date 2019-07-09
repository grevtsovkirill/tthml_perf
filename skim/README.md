## Creating skimmed version of root ntuples

##### "Pre-skimed"

Use cutflow code to skim ```ttH``` and ```ttW``` selections of 2lSS. Locted at GN2_light (origin from https://gitlab.cern.ch/DESY-ttH/ttHML_2lSS0T_CutFlow/tree/gk_ntup_skim/)
Just do: ```source run.sh```.

Output is same bulky root files as initial GN2.

##### Further skimming
```tthml_skim.cpp``` - use preskimmed files from "GN2_light" to select only branches used in the training, and create new branch for combined weight.

```weight_checks.cpp``` - check plots and application of weights. Can be used for fully skimmed or preskimmed, default weights or created by hand weight.