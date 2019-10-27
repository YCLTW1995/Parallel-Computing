package pageRank;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;


public class Parse{

    public Parse(){

    }

    public long Parse(String[] args) throws Exception {
        Configuration conf = new Configuration();

        Job job = Job.getInstance(conf, "Parse");
        job.setJarByClass(Parse.class);

//        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(ParseMapper.class);
        job.setReducerClass(ParseReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        job.setNumReduceTasks(4);
        String path1 = args[1]+"1" ;
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(path1));

        job.waitForCompletion(true);
        long N = job.getCounters().findCounter(ParseCounter.counterN).getValue() ;
    //    long Dangnode = job.getCounters().findCounter(ParseCounter.counterDangling).getValue() ;
        System.out.println("FUCKKKKKKKKKKK 1  = "+ N  );
    //    System.out.println("FUCKKKKKKKKKKK 1  = "+ Dangnode  );
        return N ;
    }

}
