package average_sort;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class AverageMapper extends Mapper<Text, Text, Text, SumCountPair> {	
	
	public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
		String num = value.toString() ;
        int intnum = Integer.valueOf(num) ;
        IntWritable writenum = new IntWritable(intnum ) ;
        
        String word = key.toString() ;

        SumCountPair SCP = new SumCountPair(intnum , 1 ) ;

        Text txtword = new Text(word) ;
        
        // key for word, value for count
		 context.write(txtword,SCP) ;     
    }
}
