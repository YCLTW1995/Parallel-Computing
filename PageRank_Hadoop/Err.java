package pageRank;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.fs.*;

import org.apache.hadoop.fs.FileSystem;
public class Err{

    public Err(){

    }

    public long  Err(String[] args ) throws Exception {
        
        Configuration conf = new Configuration();
      //  conf.set("DangPR", String.valueOf(DPR)) ;
        
        Job job = Job.getInstance(conf, "Err");
        job.setJarByClass(Err.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(ErrMapper.class);
        job.setReducerClass(ErrReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        job.setNumReduceTasks(32);
        FileSystem fs = FileSystem.get(conf);
        String inpath = args[1] + "2" ;
        String outpath = args[1] + "3" ;
        if (fs.exists(new Path(outpath))){
            fs.delete(new Path(outpath), true);
        }
        FileInputFormat.addInputPath(job, new Path(inpath));
        //FileOutputFormat.setOutputPath(job, new Path(args[1]));
        FileOutputFormat.setOutputPath(job, new Path(outpath));

        job.waitForCompletion(true);
        long ERRR = job.getCounters().findCounter(ErrCounter.Errcount).getValue() ;
        System.out.println("ERRRR  ERRRRRRRRRRRRRR    " + ERRR ) ;
        return ERRR ;
    
    }

}
