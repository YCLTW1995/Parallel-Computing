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
public class AddDang{
    public AddDang(){

    }

    public long  AddDang(String[] args, long DPR, int iteration ) throws Exception {
        
        Configuration conf = new Configuration();
        conf.set("DangPR", String.valueOf(DPR)) ;
        
        Job job = Job.getInstance(conf, "AddDang");
        job.setJarByClass(AddDang.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(AddDangMapper.class);
        job.setReducerClass(AddDangReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        job.setNumReduceTasks(4);
        iteration = 2 * iteration + 3  ;
        String itr = Integer.toString(iteration) ;
        String path1 = args[1] ;
        path1 = path1.concat(itr);

        iteration +=1 ;
/*
        String itr2 = Integer.toString(iteration) ;
        String path2 = args[1]  ;
  
        path2 = path2.concat(itr2) ;
        */
        FileSystem fs = FileSystem.get(conf);
        String inpath = args[1] + "3" ;
        String outpath = args[1] + "2" ;
        if (fs.exists(new Path(outpath))){
            fs.delete(new Path(outpath), true);
        }
        FileInputFormat.addInputPath(job, new Path(inpath));
        //FileOutputFormat.setOutputPath(job, new Path(args[1]));
        FileOutputFormat.setOutputPath(job, new Path(outpath));

        job.waitForCompletion(true);
        long ERRR = job.getCounters().findCounter(AddDangCounter.DcounterERR).getValue() ;
        System.out.println("ERRRR  ERRRRRRRRRRRRRR    " + ERRR ) ;
        return ERRR ;
    
    }

}
