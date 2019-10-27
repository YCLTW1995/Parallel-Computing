package average_sort;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.Text;

public class SortPair implements WritableComparable{
	private Text word;
	private double average;

	public SortPair() {
		word = new Text();
		average = 0.0;
	}

	public SortPair(Text word, double average) {
	    this.word = word ;
        this.average = average ;
        //TODO: constructor
	}

	@Override
	public void write(DataOutput out) throws IOException {
		word.write(out) ;
        out.writeDouble(average) ;
        
        //xxx.write(out);
		//out.writeDouble(xxx);
	}

	@Override
	public void readFields(DataInput in) throws IOException {
		//this.word = in.readFields() ;
        //this.average = in.readDOuble() ;
        word.readFields(in) ;
        average =  in.readDouble() ;
        //xxx.readFields(in);
		//xxx = in.readDouble();
	}

	public Text getWord() {
		return word;
	}

	public double getAverage() {
		return average;
	}

	@Override
	public int compareTo(Object o) {

		double thisAverage = this.getAverage();
		double thatAverage = ((SortPair)o).getAverage();

		Text thisWord = this.getWord();
		Text thatWord = ((SortPair)o).getWord();
            
        if(thisAverage > thatAverage) return -1 ;
        else if(thisAverage < thatAverage ) return 1 ;
        else {
            int result = thisWord.compareTo(thatWord) ;
            if(result < 0 ) return -1 ;
            else if(result > 0 ) return 1 ;
            else return 0 ;
        }
        
        
		// Compare between two objects
		// First order by average, and then sort them lexicographically in ascending order
	}
} 
