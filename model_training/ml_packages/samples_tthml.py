LUMI = 35900 # in pb

###
BASE       = '../../Files/ttH_Multilep/'

####
samples = {    
    'ttH' : {
        'filename' : 'mc16_13TeV.aMcAtNloPythia8EvtGen_ttH_r9364_p3832.csv',   
        'xsec'     : None,   
        'eff'      : 1.,   
        'kfactor'  : 1.,   
        'weight'   : 1.,   
        'color'    : 'black',   
    },
    'ttW' : {
        'filename' : 'mc16_13TeV.Sherpa_221_NN30NNLO_ttW_multilegNLO_r9364_p3830.csv',   
        'xsec'     : 580,   
        'eff'      : 1.,   
        'kfactor'  : 1.,   
        'weight'   : 1.,   
        'color'    : 'orange',   
    },
    # 'ttbar' : {
    #     'filename' : 'mc16_13TeV.PhPy8EG_A14_ttbar_hdamp258p75_nonallhad_r9364_p3980.roott',   
    #     'xsec'     : 47.2,   
    #     'eff'      : 1.,   
    #     'kfactor'  : 1.,   
    #     'weight'   : 1.,   
    #     'color'    : 'green',   
    # }
}
