package average_sort;

import java.io.IOException;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class AverageCombiner extends Reducer<Text, SumCountPair, Text, SumCountPair> {
    // Combiner implements method in Reducer

    public void reduce(Text key, Iterable<SumCountPair> values, Context context) throws IOException, InterruptedException {
        int totalsum = 0 ;
        int totalcount = 0 ;
        for (SumCountPair val: values) {
            int count = val.getCount() ;
            int sum = val.getSum() ;
            totalsum += sum ;
            totalcount += count ;


        }
        // write the result
        SumCountPair SCP = new SumCountPair(totalsum , totalcount) ;
        context.write(key,SCP);
    }
}
