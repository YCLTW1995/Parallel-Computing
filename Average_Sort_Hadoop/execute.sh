#!/bin/bash

# Do not uncomment these lines to directly execute the script
# Modify the path to fit your need before using this script
#hdfs dfs -rm -r /user/TA/CalculateAverage/Output/
#hadoop jar CalculateAverage.jar calculateAverage.CalculateAverage /user/shared/CalculateAverage/Input /user/TA/CalculateAverage/Output
#hdfs dfs -cat /user/TA/CalculateAverage/Output/part-*

if [[ "$1" == "" ]]; then
	iter=3
else
	iter=$1
fi

INPUT_FILE=/user/ta/Average_Sort/Input
OUTPUT_FILE=lab5/
TMP_FILE=Average_Sort/Output_tmp
JAR=Average_Sort.jar

hdfs dfs -rm -r $TMP_FILE
hdfs dfs -rm -r $OUTPUT_FILE
hadoop jar $JAR average_sort.Average_Sort $INPUT_FILE $TMP_FILE $OUTPUT_FILE $iter
hdfs dfs -getmerge $OUTPUT_FILE average_sort.txt
