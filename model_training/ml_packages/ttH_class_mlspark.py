
import sys, math
import numpy as np

import pyspark.sql
from pyspark import SparkContext, SparkConf
from pyspark.sql.functions import lit,col
from pyspark.ml.feature import VectorAssembler
from pyspark.ml.classification import GBTClassifier
from pyspark.ml.evaluation import MulticlassClassificationEvaluator
from sklearn.metrics import auc, roc_curve
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

def plot_roc_curve(fpr, tpr, auc):
    plt.figure()
    plt.plot([0,1], [0,1], 'k--', color='orange')
    plt.plot(fpr, tpr, label='auc = {:.3f}'.format(auc))
    plt.xlabel('False positive rate')
    plt.ylabel('True positive rate')
    plt.title('BDT ROC curve')
    plt.legend(loc='lower right')
    plt.grid()
    plt.savefig("Plots/SparkOut/ROC_bdt.png", transparent=True)
    
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

    gbt = GBTClassifier(labelCol='label', featuresCol='features', maxIter=50, maxDepth=10)

    print("Train BDT:")
    gbt_model = gbt.fit(train)
    pred_gbt = gbt_model.transform(test)
    pred_pd_gbt = pred_gbt.select(['label', 'prediction', 'probability']).toPandas()

    evaluator = MulticlassClassificationEvaluator( labelCol="label", predictionCol="prediction", metricName="accuracy")
    accuracy = evaluator.evaluate(pred_gbt)
    print("Accuracy: ",accuracy)

    pred_pd_gbt['probability'] = pred_pd_gbt['probability'].map(lambda x: list(x))
    pred_pd_gbt['encoded_label'] = pred_pd_gbt['label'].map(lambda x: np.eye(2)[int(x)])
    y_pred_gbt = np.array(pred_pd_gbt['probability'].tolist())
    y_true_gbt = np.array(pred_pd_gbt['encoded_label'].tolist())
    fpr_gbt, tpr_gbt, threshold_gbt = roc_curve(y_score=y_pred_gbt[:,0], y_true=y_true_gbt[:,0])
    auc_gbt = auc(fpr_gbt, tpr_gbt)
    plot_roc_curve(fpr_gbt, tpr_gbt,auc_gbt)
    print("AUC: ",auc_gbt)
    gbt_model.write().overwrite().save(path='models/bdt_spark_tth')
    
if __name__ == "__main__":
    session = pyspark.sql.SparkSession.builder.appName("Train ttH classifier").getOrCreate()
    main()
