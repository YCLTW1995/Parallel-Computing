package average_sort;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class AverageReducer extends Reducer<Text,SumCountPair,Text, DoubleWritable> {
	
    public void reduce(Text key, Iterable<SumCountPair> values, Context context) throws IOException, InterruptedException {
		// compute the average of same word
        int totalsum = 0 ; 
        int totalcount = 0 ;
		for (SumCountPair val: values) {
            int count = val.getCount() ;
            int sum = val.getSum() ;
            totalsum += sum ;
            totalcount += count ;


        }
		// write the result
        //SumCountPair SCP = new SumCountPair(sum , count) ;
		
        double ans = (double)totalsum / (double) totalcount ;
        DoubleWritable wans = new DoubleWritable() ;
        wans.set(ans) ;
        context.write(key,wans);
	}
}
