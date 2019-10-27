package average_sort;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class AveragePartitioner extends Partitioner<Text ,SumCountPair> {
        @Override
        public int getPartition(Text key, SumCountPair value, int numReduceTasks) {
            
            String word = key.toString() ;
            char char0 = word.charAt(0) ;
            if(char0 <= 'g'){
                return 0 ;
            }
            else return 1 ;
            // customize which <K ,V> will go to which reducer
	}
}
