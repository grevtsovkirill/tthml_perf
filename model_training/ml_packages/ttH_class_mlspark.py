
import sys, math

import pyspark.sql
from pyspark import SparkContext, SparkConf
from pyspark.sql.functions import lit
from samples_tthml import *
import json

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

    
        
def main():

    for s in samples:
        print(BASE,samples[s]['filename'])
    dfs=data_load(samples)
    print('There are',dfs[0].count(),' and ',dfs[1].count(),' events')
    dfsel=sel_vars(dfs)
    DF = dfsel.cache()
    DF.printSchema()

    train,test=split_ds(DF)
    
if __name__ == "__main__":
    session = pyspark.sql.SparkSession.builder.appName("Train ttH classifier").getOrCreate()
    main()
