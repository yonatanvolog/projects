import java.io.*; 
import java.util.*; 
import java.util.ArrayList;
import java.lang.ref.WeakReference;

class ArrayListApp
{
    public static void main(String[] args) throws InterruptedException
	{
		List<WeakReference<int[]>> myList = new ArrayList<WeakReference<int[]>>();

		for(int i = 0; i < 10000; ++i)
		{
			int[] array2 = new int[100000];

			WeakReference <int[]> weakReference1 = new WeakReference<int[]>(array2);

			myList.add(weakReference1);

			System.out.println("amount of free memory in the JVM = 	"
								 + Runtime.getRuntime().freeMemory()); 

		    System.out.println("amount of total memory in the JVM = "
								 + Runtime.getRuntime().totalMemory()); 

			Thread.sleep(300);
		}
    }
}


//https://javatutorial.net/weak-references-in-java
