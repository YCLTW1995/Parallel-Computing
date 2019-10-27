package wordcount;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
	
	private IntWritable result = new IntWritable();
    public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
		
		int count = 0;
		// agrregate the amount of same starting character
		for (IntWritable val: values) {
		    count += val.get() ;
        
        }
        result.set(count) ;
        context.write(key,result) ;
		// write the result
		//context.write(K,V);
		
	}
}
