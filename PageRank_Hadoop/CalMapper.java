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


public class CalMapper extends Mapper<Text, Text, Text, Text> {

    @Override
    public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        Configuration conf = context.getConfiguration();
        long NN = Long.parseLong(conf.get("NCount"));

        String inputline = value.toString() ;
        Pattern PRpattern = Pattern.compile("(.+?)~~@@PR@@~~");
        Matcher PRmatch = PRpattern.matcher(inputline) ;
        String strPR ;
        if(PRmatch.find()) {
            strPR = PRmatch.group(1) ;

            String [] splits = inputline.split("~~@@PR@@~~") ;


            if(splits.length != 1 ) {  ////// if this is not dangling node 
                String inputline2 = splits[1] ;
                Pattern LKpattern = Pattern.compile("(.+?)!!!##!!!##!!!");
                Matcher LKmatch = LKpattern.matcher(inputline2) ;
                double oldPR = Double.parseDouble(strPR) ;
                double count = 0 ;    
                while(LKmatch.find()){
                    count++ ;
                }
                LKpattern = Pattern.compile("(.+?)!!!##!!!##!!!");
                LKmatch = LKpattern.matcher(inputline2) ;
                while(LKmatch.find()){
                    String link = LKmatch.group(1) ;
                    double newPR = oldPR/count ;
                    String newPRIN = Double.toString(newPR) ;
                    newPRIN = newPRIN.concat("!!!##!!!##!!!");
                    newPRIN = newPRIN.concat(key.toString()) ;
                    Text linkIN = new Text(link) ;
                    Text PRIN = new Text(newPRIN) ;
                    context.write(linkIN , PRIN) ;
                }

            }
            else {    ///// this is dangling node 
                //// add Dangling node PR to counter 
                double oldPR = Double.parseDouble(strPR) ;
                String NNN = NN + "";
                double dd = Double.parseDouble(NNN);
                oldPR = oldPR / dd ;
                //  context.getCounter(PageRankCounter.counterDangPR).increment((long)(oldPR*1E18));
                //context.getCounter(PageRankCounter.counterDangPR).increment(1);

                //  write to reducer
                //
                /* 
                   String ISDANG = "--++DANG++--";

                   Text txtISDANG = new Text(ISDANG) ;
                   context.write(key , txtISDANG) ;
                   */
                //
                //

                String   ISDANG = "~~@@DNG@@~~";
                Text TO = new Text(ISDANG) ;
                String prpr = Double.toString(oldPR) ;
                ISDANG = ISDANG.concat(prpr) ;
                ISDANG = ISDANG.concat("~~@@PR@@~~");
                Text   txtISDANG = new Text(ISDANG) ;
                context.write( TO, txtISDANG) ;
                System.out.println("Hello World   "  + TO.toString() + "    " + txtISDANG.toString());                   
  
            }
        }
        context.write(key,value);
    }
}
