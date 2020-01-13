
import sys, math

import pyspark.sql
from pyspark import SparkContext, SparkConf
from pyspark.sql.functions import lit
from samples_tthml import *

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

def main():

    for s in samples:
        print(BASE,samples[s]['filename'])
    data_load(samples)
        
if __name__ == "__main__":
    session = pyspark.sql.SparkSession.builder.appName("Train ttH classifier").getOrCreate()
    main()
