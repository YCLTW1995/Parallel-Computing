#!/usr/bin/bash

INPUT_FILE=/user/ta/PageRank/Input/input-50G
PAGERANKFILE=/user/p103062333/PageRank
OUTPUT_FILE=PageRank/Output
OUTPUT_FILE2=PageRank/Output2
OUTPUT_FILE3=PageRank/Output3
OUTPUT_FILE4=PageRank/Output4
JAR=PageRank.jar

hdfs dfs -rm -r $PAGERANKFILE
hadoop jar $JAR pageRank.PageRank $INPUT_FILE $OUTPUT_FILE 21
hdfs dfs -getmerge $OUTPUT_FILE pagerankjob.txt
hdfs dfs -rm -r $PAGERANKFILE
