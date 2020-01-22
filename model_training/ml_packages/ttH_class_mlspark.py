
import sys, math

import pyspark.sql
from pyspark import SparkContext, SparkConf
from pyspark.sql.functions import lit,col
from pyspark.ml.feature import VectorAssembler
from samples_tthml import *
import json
import matplotlib.pyplot as plt

def data_load(in_list):
    DFList = [] 
    label_count=0
    for s in samples:
        dsPath = BASE+samples[s]['filename']
        print('Loading {0} sample file'.format(s),'\n',dsPath )   
        tempDF =session.read\
                          .format('csv')\
                          .option('header', 'true')\
                          .option('inferschema', 'true')\
                          .load(dsPath)
        tempDF=tempDF.withColumn('sample', lit(s)) 
        tempDF=tempDF.withColumn('label', lit(label_count)) 
        label_count+=1
        DFList.append(tempDF)
    return DFList

def sel_vars(df,list_name="../VarList_30.json"):
    with open("../VarList_30.json") as vardict:
        variablelist = json.load(vardict)[:]    
    variablelist.append("sample")
    variablelist.append("label")

    DF = df[0].select(variablelist)
    for df_ in df[1:]:
        DF = DF.union(df_.select(variablelist))
    print( 'Partitions: {}'.format(DF.rdd.getNumPartitions()))
    return DF


def split_ds(df,train_frack=0.9):
    balance_of_classes = df.filter(df['label'] == 0).count()/df.filter(df['label'] == 1).count()
    if not(balance_of_classes > 0.5 and balance_of_classes < 1.5):
        print("Not balanced ds, double check your split")
        return None,None

    print("rather balanced ds. split to it into train and test dataframes.")
    train, test = df.randomSplit([train_frack,1-train_frack])
    return  train, test       

def compute_hist(data, feature, target='label', n_bins=100, x_lim=[0,3]):
        
    ## Fix the range
    data = data.where((col(feature)<=x_lim[1]) &
                      (col(feature)>=x_lim[0]))
    
    sgn = data.where(col(target)==1.0) 
    bkg = data.where(col(target)==0.0)

    ## Compute the histograms
    bins_sgn, counts_sgn = sgn.select(feature).rdd.flatMap(lambda x: x).histogram(n_bins)
    bins_bkg, counts_bkg = bkg.select(feature).rdd.flatMap(lambda x: x).histogram(n_bins)
    
    return (bins_sgn, counts_sgn), (bins_bkg, counts_bkg)    


def save_hist(hist_signal, hist_bkg,feature="Mll01",xlab='$m_{ll}$',ptitle="Distribution of $m_{ll}$"):
    f, ax = plt.subplots()
    ax.hist(hist_signal[0][:-1], bins=hist_signal[0], weights=hist_signal[1], alpha=0.5, label='signal')
    ax.hist(hist_bkg[0][:-1], bins=hist_bkg[0], weights=hist_bkg[1], alpha=0.5, label='background')
    ax.set_xlabel(xlab)
    ax.set_ylabel('counts')
    ax.set_title(ptitle)
    ax.legend()
    plt.savefig("Plots/SparkOut/"+feature+".png", transparent=True)


def main():

    for s in samples:
        print(BASE,samples[s]['filename'])
    dfs=data_load(samples)
    print('There are',dfs[0].count(),' and ',dfs[1].count(),' events')
    dfsel=sel_vars(dfs)
    DF = dfsel.cache()
    DF.printSchema()

    train,test=split_ds(DF)
    hist_signal, hist_bkg = compute_hist(data=train, feature='Mll01', target='label', n_bins=50, x_lim=[0,500000])
    save_hist(hist_signal, hist_bkg)

    feature_to_train = train.columns
    feature_to_train.remove('label')
    feature_to_train.remove('sample')

    
    assembler = VectorAssembler(inputCols=feature_to_train, outputCol='features')
    train = assembler.transform(train)
    test = assembler.transform(test)
    
    
if __name__ == "__main__":
    session = pyspark.sql.SparkSession.builder.appName("Train ttH classifier").getOrCreate()
    main()
