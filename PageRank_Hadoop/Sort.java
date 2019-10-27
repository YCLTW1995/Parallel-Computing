package pageRank;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;

 import org.apache.hadoop.io.NullWritable;
public class Sort{

    public Sort(){

    }

    public void  Sort(String[] args, int iteration ) throws Exception {
        Configuration conf = new Configuration();

        Job job = Job.getInstance(conf, "Sort");
        job.setJarByClass(Sort.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(SortMapper.class);
        job.setPartitionerClass(SortPartitioner.class);
        job.setReducerClass(SortReducer.class);

        job.setMapOutputKeyClass(SortStruct.class);
        job.setMapOutputValueClass(NullWritable.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);

        job.setNumReduceTasks(1);
        String itr = Integer.toString(iteration) ;
        
        String path1 = args[1]+ "2"  ;

        FileInputFormat.addInputPath(job, new Path(path1));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        
        job.waitForCompletion(true);


    }

}
