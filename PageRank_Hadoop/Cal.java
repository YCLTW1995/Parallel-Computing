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

public class Cal{

    public Cal(){

    }

    public long  Cal(String[] args, long Nvalue, int iteration ) throws Exception {
        Configuration conf = new Configuration();
        conf.set("NCount", String.valueOf(Nvalue)) ;
        
        Job job = Job.getInstance(conf, "Cal");
        job.setJarByClass(Cal.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(CalMapper.class);
        job.setReducerClass(PageRankReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);
        
        job.setNumReduceTasks(32);
        iteration = 2 * iteration + 2 ;
        String itr = Integer.toString(iteration);
        String path1 = args[1];
        path1 = path1.concat(itr) ;
        /*
        iteration +=1 ;
        String itr2 = Integer.toString(iteration);
        String path2 = args[1] ;
        path2 = path2.concat(itr2) ;
        */
        FileSystem fs = FileSystem.get(conf) ;
        String inpath = args[1] + "2" ;
        String outpath = args[1] + "3";
        if (fs.exists(new Path(outpath))) {
            fs.delete(new Path(outpath), true);
        }
        FileInputFormat.addInputPath(job, new Path(inpath));
        //FileOutputFormat.setOutputPath(job, new Path(args[1]));
        FileOutputFormat.setOutputPath(job, new Path(outpath));

        job.waitForCompletion(true);
        long DANGPR = job.getCounters().findCounter(PageRankCounter.counterDangPR).getValue() ;
        System.out.println("ERRRR  ERRRRRRRRRRRRRR    " + DANGPR ) ;
        return DANGPR ;
    
    }

}
