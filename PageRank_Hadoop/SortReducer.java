package pageRank ;

import java.io.IOException;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.io.NullWritable;

public class SortReducer extends Reducer<SortStruct, NullWritable, Text, DoubleWritable> {

    public void reduce( SortStruct key, Iterable<NullWritable> value, Context context) throws IOException, InterruptedException {

            double aver = key.getPR() ;
            Text word = key.getword() ;
            DoubleWritable ans = new DoubleWritable() ;
            ans.set(aver) ;
           // context.write(word,ans);
            context.write(key.getword() , new DoubleWritable(key.getPR()));

            System.out.println("PR IS = " + word.toString() + "  " + aver);
    }
}

