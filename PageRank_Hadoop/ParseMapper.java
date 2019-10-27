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



public class ParseMapper extends Mapper<LongWritable, Text, Text, Text> {

    public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {

        //StringTokenizer itr = new StringTokenizer(value.toString());
        //while (itr.hasMoreTokens()) {
        //  String inputline = itr.nextToken();
        String inputline = value.toString() ;   
        Pattern titlePattern = Pattern.compile("<title>(.+?)</title>");
        Matcher titleMatcher = titlePattern.matcher(inputline);
        Pattern linkPattern = Pattern.compile("\\[\\[(.+?)([\\|#]|\\]\\])");
        Matcher linkMatcher = linkPattern.matcher(inputline);
        Text title = new Text() ;
        while(titleMatcher.find()) {
            String strtitle = "" ;
            strtitle = titleMatcher.group(1) ;
            String newstrtitle = unescapeXML(strtitle) ;
            title.set(newstrtitle) ;
        }  

        //this is formal write 
        while(linkMatcher.find()) {
            String strlink = linkMatcher.group(1) ;
            String newstrlink = capitalizeFirstLetter(strlink) ;
            newstrlink = unescapeXML(newstrlink);
            Text link = new Text(newstrlink) ;
            context.write(link, title );
        } 
        
        String isnode = "!!!##!!!##!!!" ;
        Text Isnode = new Text(isnode) ;
        context.write(title , Isnode ) ;



        // }
        /*
        //  Match title pattern  
        Pattern titlePattern = Pattern.compile("<title>(.+?)</title>");
        // Matcher titleMatcher = titlePattern.matcher(xxx);
        // No need capitalizeFirstLetter

        //  Match link pattern 
        Pattern linkPattern = Pattern.compile("\\[\\[(.+?)([\\|#]|\\]\\])");
        // Matcher linkMatcher = linkPattern.matcher(xxx);
        // Need capitalizeFirstLetter
        */	
    }

    private String unescapeXML(String input) {

        return input.replaceAll("&lt;", "<").replaceAll("&gt;", ">").replaceAll("&amp;", "&").replaceAll("&quot;", "\"").replaceAll("&apos;", "\'");

    }

    private String capitalizeFirstLetter(String input){

        char firstChar = input.charAt(0);

        if ( firstChar >= 'a' && firstChar <='z'){
            if ( input.length() == 1 ){
                return input.toUpperCase();
            }
            else
                return input.substring(0, 1).toUpperCase() + input.substring(1);
        }
        else 
            return input;
    }
}
