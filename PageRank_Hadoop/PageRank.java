package pageRank;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class PageRank{

    public static void main(String[] args) throws Exception {
        
        String testcase = args[0] ;
        int MAX = 0 ;
        if(testcase.contains("100M")){
            MAX = 21 ;
            //System.out.println("!!!!!!!!!!!!!!!!!!!!Main JAVA MAX  " + MAX) ;
        }
        else if(testcase.contains("1G")){
          MAX = 22 ;  
        }
        else if(testcase.contains("10G")){
            MAX = 22 ;
        }
        else if(testcase.contains("50G")){
            MAX = 16 ;
        }

        Parse job1 = new Parse();
        long Nvalue = job1.Parse(args);
        System.out.println("!!!!!!!!!!!!!!!!!!!!Main JAVA N is  " + Nvalue);
        
        Dangling job2 = new Dangling() ;
        long Dangvalue = job2.Dangling(args, Nvalue) ;
        System.out.println("!!!!!!!!!!!!!!!!!!!!Main JAVA DANGLING is  " + Dangvalue);
        int iteration = Integer.parseInt(args[2]) ;
        int xx = 0 ;
        int isbreak = 0 ;
        for( xx = 0 ; xx < iteration ; xx ++ ){
            Cal job3 = new Cal();
            long dangpr = job3.Cal(args, Nvalue,xx);

            AddDang job4 = new AddDang() ;
            long err = job4.AddDang(args , dangpr,xx  ) ;
            double check = (double)(err)/(double)(1E18);
            System.out.println("~~~~~~~~~ERR VALUE is  " + check );
            if (check < 0.001 ){
                isbreak = 1 ;
                break ;
            } 
            if(xx + 1 == MAX ) break ;
        }
        if(isbreak != 1 )  xx -= 1 ;
        Sort job5 = new Sort() ;
        job5.Sort(args, xx*2 + 4  ) ;
       

        System.exit(0);
    }
}
