package average_sort;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Average_Sort{
	
    	public static void main(String[] args) throws Exception {  
        
		/* Don't need to modify this file.
 		   InputDir : args[0]
 		   TmpDir : args[1]
 		   OutputDir : args[2] 
		   Number of reducer for Sort : args[3] */
	
		//Job 1: Calculate Average
		CalculateAverage job1 = new CalculateAverage();
                job1.CalculateAverage(args);

		//Job 2: Sort
		Sort job2 = new Sort();
		job2.Sort(args);
		
		System.exit(0);
    	}  
}
