package pageRank ;


import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.io.NullWritable;

public class SortPartitioner extends Partitioner<SortStruct, NullWritable> {

    @Override
    public int getPartition(SortStruct key, NullWritable value, int numReduceTasks) {
        return 0 ;
    }
}
