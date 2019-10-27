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

public class Dangling{

    public Dangling(){

    }

    public long  Dangling(String[] args, long N ) throws Exception {
        
        
        Configuration conf = new Configuration();
        conf.set("DangPR", String.valueOf(N)) ;
        Job job = Job.getInstance(conf, "Dangling");
        job.setJarByClass(Dangling.class);
        
        System.out.println("DANG PR by job3  "+ N  );

        

        job.setInputFormatClass(KeyValueTextInputFormat.class);

        job.setMapperClass(DanglingMapper.class);
        job.setReducerClass(DanglingReducer.class);

        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        job.setNumReduceTasks(4);
        /*
        FileSystem hdfs = FileSystem.get(conf) ;
        Path output = new Path ( args[0]) ;
        if(hdfs.exists(output)){
            hdfs.delete(output,true) ;
        }
        */
        String path1 = args[1]+"1";
        String path2 = args[1] + "2" ;
        FileInputFormat.addInputPath(job, new Path(path1));
    //    FileOutputFormat.setOutputPath(job, new Path(args[1]));
        FileOutputFormat.setOutputPath(job, new Path(path2));

        job.waitForCompletion(true);
        long Dangnode = job.getCounters().findCounter(DanglingCounter.counterDang).getValue() ;
        System.out.println("FUCKKKKKKKKKKK 1  = "+ Dangnode  );
        return Dangnode ;
    }

}

