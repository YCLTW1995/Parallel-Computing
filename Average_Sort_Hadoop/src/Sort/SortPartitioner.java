package average_sort;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.io.NullWritable;

public class SortPartitioner extends Partitioner<SortPair, NullWritable > {

    private double maxValue = 11.05;
    private double minValue = 9.0;

    @Override
    public int getPartition(SortPair key, NullWritable value, int numReduceTasks) {
        double interval = (11.05-9.0) / (double) numReduceTasks ;	
        int num = 0;  
        double val ;
        val = key.getAverage() ;
        int returnwhat = numReduceTasks - 1 ;
        for(int i = 1 ; i <= numReduceTasks; i ++ ){
            if(val >= (double)((i-1)*interval + 9.0 )  && val < (double)(i* interval + 9)){
                int re = numReduceTasks  ;
                return  re - i ;
            }
        }
        // customize which <K ,V> will go to which reducer
        // Based on defined min/max value and numReduceTasks
        return 0 ;
        
        //return num;
    }
}
