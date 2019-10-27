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

import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class DanglingMapper extends Mapper<Text, Text, Text, Text> {

    @Override
    public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        String vvalue = value.toString() ;
//        System.out.println("Hello   " + vvalue);
        context.write(key,value) ;    
    }
}

