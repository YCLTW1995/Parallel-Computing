package average_sort;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.NullWritable;

public class SortMapper extends Mapper<Text, Text, SortPair,  NullWritable> {
	
	//private SortPair sp;
	
	public void map(Text key, Text value, Context context) throws IOException, InterruptedException {
        String num = value.toString() ;
        double doublenum = Double.parseDouble(num) ;
        String word = key.toString() ;

        Text txtword = new Text(word) ;
        SortPair SP = new SortPair(txtword , doublenum ) ;
        context.write(SP , NullWritable.get()) ;
		//context.write(xxx, NullWritable.get());
	}
	
}
