package pageRank ;


import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.Text;

public class SortStruct implements WritableComparable{
    private double PR;
    private Text title ;
    public SortStruct() {
        title = new Text() ;
        PR = 0.0;
    }

    public SortStruct(Text title , double in) {
        this.title = title ;
        this.PR = in ;
    }

    public Text getword(){
        return title ;
    }
    public double getPR() {
        return PR;
    }
    @Override
    public void write(DataOutput out) throws IOException {
        title.write(out) ;
        out.writeDouble(PR) ;
    }

    @Override
    public void readFields(DataInput in) throws IOException {
        this.title.readFields(in) ;
        this.PR =  in.readDouble() ;
    }
    @Override
    public int compareTo(Object o) {

        double thisAverage = this.getPR();
        double thatAverage = ((SortStruct)o).getPR();

        Text thisword = this.getword() ;
        Text thatword = ((SortStruct)o).getword();
        if(thisAverage > thatAverage) return -1 ;
        else if(thisAverage < thatAverage ) return 1 ;
        else return thisword.toString().compareTo(thatword.toString());


    }
} 
