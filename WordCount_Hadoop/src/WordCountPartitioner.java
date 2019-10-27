package wordcount;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class WordCountPartitioner extends Partitioner<Text, IntWritable> {
	@Override
	public int getPartition(Text key, IntWritable value, int numReduceTasks) {
		// customize which <K ,V> will go to which reducer
		
        String word = key.toString() ;
        char result1 = word.charAt(0) ; 
        char up = Character.toUpperCase(result1) ;
        if(up <= 'G') {
            return 0 ;
        }
        else return 1 ;
        /*
        if(result1 <= 'g' ) {
            return 0 ;
        }
        else return 1;
        */
	}
}
