package average_sort;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.io.NullWritable;

public class SortReducer extends Reducer<SortPair, NullWritable, Text, DoubleWritable> {

    public void reduce( SortPair key, Iterable<NullWritable> value, Context context) throws IOException, InterruptedException {
		// output the word and average

       // for (SortPair val: key) {
            double aver = key.getAverage() ;
            Text word = key.getWord() ;
            DoubleWritable ans = new DoubleWritable() ;
            ans.set(aver) ;

			context.write(word,ans);
		//}
	}
}
