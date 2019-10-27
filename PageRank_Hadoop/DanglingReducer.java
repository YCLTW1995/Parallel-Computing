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
import org.apache.hadoop.mapreduce.Reducer;
import java.util.ArrayList;
import java.util.Arrays;
import java.net.URI;
import java.io.*;
import java.lang.*;

public class DanglingReducer extends Reducer<Text, Text, Text, Text> {
    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        boolean isfirst = true ;
        int cnt = 0 ;
        int haveother = 0 ;
        Configuration conf = context.getConfiguration();
        long NN = Long.parseLong(conf.get("DangPR"));
        String NNN = NN + "";
        double dd = Double.parseDouble(NNN) ;
        double PR = (double)1.0/ dd ;
        
        System.out.println("Hello World"  + PR  );
        
        String strPR = Double.toString(PR) ;
        StringBuffer PRandLKs = new  StringBuffer(strPR); 
        PRandLKs.append("~~@@PR@@~~") ;
        for(Text value : values){
            cnt ++ ;
            String strvalue = value.toString() ;
            if(strvalue.equals("!!!##!!!##!!!")){
                strvalue = "";
            }
            else{
                haveother = 1 ;
                //      if(isfirst == false){
                PRandLKs.append(strvalue) ;
                PRandLKs.append("!!!##!!!##!!!") ;
                //    }
                isfirst = false ;
            }
            //    PRandLKs.append(strvalue) ;
        }
        if(haveother  == 0 ) {
            context.getCounter(DanglingCounter.counterDang).increment(1) ;
        }
        String STRPRandLKS = PRandLKs.toString() ;    
        context.write(new Text(key), new Text(STRPRandLKS));

    }

}
