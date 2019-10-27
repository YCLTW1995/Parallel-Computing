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
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.conf.Configuration;

import java.util.ArrayList;
import java.util.Arrays;
import java.net.URI; 
import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.NullWritable;
import java.lang.StringBuffer;



public class ParseReducer extends Reducer<Text, Text, Text, Text> {
    @Override
    public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        int isnode = 0 ; 
        StringBuffer TMP = new StringBuffer() ;
        int cnt = 0 ;
        int cnt2 = 0 ;
        for(Text value : values) {
            cnt ++ ;
            String strvalue = value.toString() ;
            if(strvalue.equals("!!!##!!!##!!!")){
                isnode = 1 ;
            }
            TMP.append(strvalue);
            TMP.append("@@@~~~@@@");
        }

       // System.out.println("TMP is     " + TMP.toString() );
        if(isnode ==1 ) {

            context.getCounter(ParseCounter.counterN).increment(1) ;
            String strTMP = TMP.toString() ;
            Pattern patting = Pattern.compile("(.+?)@@@~~~@@@");
            Matcher matching = patting.matcher(strTMP) ;

            while(matching.find()){
                cnt2 ++ ;
                String val = "";
                val = matching.group(1) ;
                if(val.equals("!!!##!!!##!!!")){
                    String link = "!!!##!!!##!!!" ;
                    Text txtlink = new Text(link) ;
                    context.write(key , txtlink) ;
                }
                else {
                    Text txtlink = new Text(val) ;
                    String ss = key.toString() ;
                    Text Key = new Text(ss) ;
                    context.write(txtlink , Key) ;
                }
            }
       // System.out.println("CNT IS   " + cnt + "   CNT2 is   " + cnt2 + "    " + TMP.toString() );
        }
       // System.out.println("CNT IS   " + cnt + "   CNT2 is   " + cnt2 + "    " + TMP.toString() );
    }
}
