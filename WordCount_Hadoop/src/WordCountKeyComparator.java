package wordcount;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;

public class WordCountKeyComparator extends WritableComparator {
	
	public WordCountKeyComparator() {
		super(Text.class, true);
	}	
	

	public int compare(WritableComparable o1, WritableComparable o2) {
		Text key1 = (Text) o1;
		Text key2 = (Text) o2;
        String word1 = key1.toString() ;
        String word2 = key2.toString() ;
        char result1 = word1.charAt(0) ;
        char result2 = word2.charAt(0) ;
        char upre1 = Character.toUpperCase(result1);
        char upre2 = Character.toUpperCase(result2);
        if(upre1 >  upre2 ) {
             return 1 ;
        }
        else if( upre1 < upre2 ) {
            return -1 ;

        }
        else {
        if(result1 > result2 )  return 1 ;
        else if(result2 > result1 ) return -1 ;
        else return 0 ;
        }
        
        

		// TODO Order by A -> a -> B -> b ....

	}
}
