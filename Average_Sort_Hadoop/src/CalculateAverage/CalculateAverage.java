package average_sort;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;


public class CalculateAverage{
	
	public CalculateAverage(){

  	}
	
	public void CalculateAverage(String[] args) throws Exception {
		Configuration conf = new Configuration();
		
		Job job = Job.getInstance(conf, "CalculateAverage");
		job.setJarByClass(CalculateAverage.class);

		// set the inputFormatClass <K, V>
		job.setInputFormatClass(KeyValueTextInputFormat.class);		
		
		// set the class of each stage in mapreduce
		job.setMapperClass(AverageMapper.class);
		job.setCombinerClass(AverageCombiner.class);
		job.setPartitionerClass(AveragePartitioner.class);
		job.setReducerClass(AverageReducer.class);
		
		// set the output class of Mapper and Reducer
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(SumCountPair.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(DoubleWritable.class);
		
		// set the number of reducer
		job.setNumReduceTasks(2);
		
		// add input/output path
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.waitForCompletion(true);
	}
	
}
