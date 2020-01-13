
import sys, math
 
from pyspark import SparkContext, SparkConf
from pyspark.sql.functions import lit
from samples_tthml import *

def main():
    # create Spark context with Spark configuration
    conf = SparkConf().setAppName("Train ttH classifier")
    sc = SparkContext(conf=conf)

    for s in samples:
        print(BASE,samples[s]['filename'])
if __name__ == "__main__":
    main()
