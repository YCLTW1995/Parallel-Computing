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


public class AddDangReducer extends Reducer<Text, Text, Text, Text> {
    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        Configuration conf = context.getConfiguration();
        long dangpr = Long.parseLong(conf.get("DangPR"));
        String strnewpr = "";
        String stroldpr = "";


        for(Text val : values) {
            System.out.println(" !!!!!!!!!!!!!!!!  " + val.toString());
            String value = val.toString() ;
            Pattern PRpattern = Pattern.compile("(.+?)&&--PRSEP--&&");
            Matcher PRmatch = PRpattern.matcher(value) ;
            if(PRmatch.find()){
                strnewpr = PRmatch.group(1);
            }
            Pattern oldPRpattern = Pattern.compile("&&--PRSEP--&&(.+?)~~@@PR@@~~");
            Matcher oldPRmatch = oldPRpattern.matcher(value) ;
            if(oldPRmatch.find()){
                stroldpr = PRmatch.group(1);
            }
            double newpr = Double.parseDouble(strnewpr) ;
            double oldpr = Double.parseDouble(stroldpr) ;
            String NNN = dangpr + "";
            double dd = Double.parseDouble(NNN);
            double tmppr = dd / (double)1E18 ;
            newpr += ((double)tmppr*(double)0.85) ;
            double err = newpr - oldpr ;
            if(err < 0 ) err = -err ;
            double tmp = err*1E18 ;
            long lltmp = (long)tmp ;
            System.out.println("OldPR ==   " + oldpr + "    NEWPR     ==   " + newpr ) ;
            context.getCounter(AddDangCounter.DcounterERR).increment(lltmp) ;

            //////write back ;
            String []sep = value.split("~~@@PR@@~~");
            if(sep.length == 1 ){
                String NEWPR = Double.toString(newpr);
                NEWPR = NEWPR.concat("~~@@PR@@~~");
                Text INNEW = new Text(NEWPR) ;
                context.write(key ,INNEW);
            }
            else{
                String info = sep[1] ;
                    String NEWPR = Double.toString(newpr);
                    NEWPR = NEWPR.concat("~~@@PR@@~~");
                    NEWPR = NEWPR.concat(info);
                    Text INNEW = new Text(NEWPR) ;
                    context.write(key ,INNEW);

            }
            //
            //
        }




    }

}
//public enum PageRankCounter{DONE , NOTDONE} 
