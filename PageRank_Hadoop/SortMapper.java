package pageRank;
import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;

import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.conf.Configuration;

import java.util.ArrayList;
import java.util.Arrays;
import java.net.URI;
import java.io.*;

import org.apache.hadoop.io.NullWritable;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;


public class SortMapper extends Mapper<Text, Text, SortStruct, NullWritable> {

    @Override
    public void map(Text key, Text value, Context context) throws IOException, InterruptedException {

        String inputline = value.toString() ;
        String [] parts = inputline.split("~~@@PR@@~~") ;
        String strPR = "";
        strPR = parts[0] ;
        double PR = Double.parseDouble(strPR) ;
        
        SortStruct SS = new SortStruct ( key, PR ) ;
        context.write(SS , NullWritable.get()) ;
        System.out.println("PR IS = " + SS.getword() + "  " + SS.getPR() );

    }
}
