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


public class ErrReducer extends Reducer<Text, Text, Text, Text> {
    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        Configuration conf = context.getConfiguration();
        String strnewpr = "";
        String stroldpr = "";

        String strkey = key.toString() ;
        if(strkey.contains("~~@@ERR@@~~")){
            double sum = 0.0 ;
            for(Text val : values){
                String strv = val.toString() ;
                double dvalue = Double.parseDouble(strv) ;
                sum += dvalue ;
            }
            double in = sum * 1E18 ;
            long iin = (long)in ;
            context.getCounter(ErrCounter.Errcount).increment(iin);
        }
        else{
            for(Text val : values){
            context.write(key,val);
            }
        }




    }

}
//public enum PageRankCounter{DONE , NOTDONE} 
