package wordcount;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class WordCountMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
	
	private IntWritable one = new IntWritable(1);
	
    public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
		
		// we simply use StringTokenizer to split the words for us.
		StringTokenizer itr = new StringTokenizer(value.toString());
		while (itr.hasMoreTokens()) {
			// TODO: find the first character of word, and create K,V pair for it.
			// we only need to handle Aa~Zz
			String toProcess = itr.nextToken();
            char result = toProcess.charAt(0) ;
            if(result <= 'z' && result >= 'A' && result != '[' && result != ']' ) {
            String inin = String.valueOf(result) ;
            Text word = new Text(inin) ;

			// create <K, V> pair
			context.write(word,one);
                
            }

		}
	}
}
