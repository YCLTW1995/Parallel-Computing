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


public class PageRankReducer extends Reducer<Text, Text, Text, Text> {
    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        Configuration conf = context.getConfiguration();
        long NN = Long.parseLong(conf.get("NCount"));
        String keyinput = key.toString() ;
        double sum = 0 ;
        //System.out.println("CHECK KEY" + key.toString()+"DONE");
        if(keyinput.contains("~~@@DNG@@~~")){
            for(Text dpr : values){
                String strtmp = dpr.toString() ;
                Pattern DDpattern = Pattern.compile("(.+?)~~@@PR@@~~");
                Matcher DDmatch = DDpattern.matcher(strtmp);
                if(DDmatch.find()){

                    String tt = DDmatch.group(1);
                    String []sep = tt.split("~~@@DNG@@~~");
                    String mean = sep[1] ;
                    double dmean = Double.parseDouble(mean) ;
                    sum+= dmean ;
                    System.out.println("CHECK KEY~~~~~~~~~~~~~~~~~~~     " + tt + "       "  + mean + "        " + dmean );
                    /*
                    Pattern SSpattern = Pattern.compile("~~@@DNG@@~~(.+?)");
                    Matcher SSmatch = SSpattern.matcher(tt);
                    if(SSmatch.find()){
                        String mean = SSmatch.group(1);
                        double dmean = Double.parseDouble(mean) ;
                        sum += dmean ;
                        
                        System.out.println("CHECK KEY~~~~~~~~~~~~~~~~~~~     " + tt + "       "  + mean + "        " + dmean );
                    }
                    */

                }
            }

            double tmp = sum*1E18 ;
            long lltmp = (long)tmp ;
            System.out.println("i!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Hello World    "+ "    " + sum + "    "  +  tmp +  "     " + lltmp );
            context.getCounter(PageRankCounter.counterDangPR).increment(lltmp);
        }
        else{
            //    System.out.println("NOTHERE" );
            String stroldPR = "";
            double sumPR  = 0 ;
            String originalinput ="" ;
            for(Text value : values) {
                String val = value.toString() ;
                if(val.contains("~~@@PR@@~~")){   // find old PR

                    Pattern PRpattern = Pattern.compile("(.+?)~~@@PR@@~~");
                    Matcher PRmatch = PRpattern.matcher(val) ;
                    if(PRmatch.find()){
                        stroldPR = PRmatch.group(1);

                        originalinput = val ;
                    }
                }
                else if (val.contains("!!!##!!!##!!!")){
                    String [] splits = val.split("!!!##!!!##!!!") ;
                    String tmppr = splits[0] ;
                    double dtmppr =  Double.parseDouble(tmppr) ;
                    sumPR += dtmppr ;
                }
            }

            //long dangPR = context.getCounters().findCounter(PageRankCounter.counterDangPR).getValue() ;
            //double dangling = dangPR/1E16; 
            String NNN = NN + "";
            double dd = Double.parseDouble(NNN); 
            double newPR = 0.15/dd ;
            newPR += (double)(0.85*sumPR) ;
            //newPR += ((double)dangling/(double)NN) ;

            double oldPR = Double.parseDouble(stroldPR) ;

            double err = newPR-oldPR ;
            if(err < 0 ) err = -err ;

            // context.getCounter(PageRankCounter.counterERR).increment((long)( err*(long)1E16));


            ////add new PR and write back 

            String newIN = Double.toString(newPR) ;
            newIN = newIN.concat("&&--PRSEP--&&");
            newIN = newIN.concat(originalinput);
            Text writetxt = new Text(newIN) ;
            context.write(key, writetxt) ;
        }
    }

}
//public enum PageRankCounter{DONE , NOTDONE} 
