#!/bin/bash

# Do not uncomment these lines to directly execute the script
# Modify the path to fit your need before using this script

INPUT_FILE=/user/ta/WordCount/Input
OUTPUT_FILE=lab5/
JAR=WordCount.jar

hdfs dfs -rm -r $OUTPUT_FILE
hadoop jar $JAR wordcount.WordCount $INPUT_FILE $OUTPUT_FILE
hdfs dfs -getmerge $OUTPUT_FILE wordcount.txt
