
import sys, math
 
from pyspark import SparkContext, SparkConf
 
if __name__ == "__main__":
 
  # create Spark context with Spark configuration
  conf = SparkConf().setAppName("Train ttH classifier")
  sc = SparkContext(conf=conf)
