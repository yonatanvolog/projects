package il.co.ilrd.pair;

import java.util.Comparator;
import java.util.Map;
import java.util.Objects;

public class Pair<T,U> implements Map.Entry<T,U> {
	
	private T first;
	private U second;
	
	private Pair(T first, U second) {
		this.first =  first;
		this.second = second;
	};
	
	public static<T,U> Pair<T,U> of(T first, U second){	       //factory method
		return new Pair<T, U>(first, second);
	}
	
	public static<T,U> Pair<U,T> swap(Pair<T,U> pair){
		return new Pair<U, T>(pair.second, pair.first);
	}
	
	private static<T> void swapInArr(T[] array, int i, int j) {
		T temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
	
	private static<T> void halfSortArray(T[] array, MyComperator<T> lambda) {
		for(int i = 0, j = array.length -1; j > i; ++i, --j) {
			if(0 < lambda.compare(array[i], array[j])) {
				Pair.swapInArr(array, i, j);
			}
		}
	}
	
	private static<T> T FindMinInRange(T[] array, int start, int end 
											 	   ,MyComperator<T> lambda) {
		T min = array[start];
		for(int i = start; i < end; ++i) {
			if(0 < lambda.compare(min, array[i])) {
				min = array[i];
			}
		}
		
		return min;
	}
	
	private static<T> T FindMaxInRange(T[] array, int start, int end 
												   ,MyComperator<T> lambda) {
		T max = array[start];
		for(int i = start; i < end; ++i) {
			if(0 > lambda.compare(max, array[i])) {
				max = array[i];
			}
		}
		
		return max;
	}
	
	private static<T> Pair<T,T> genericFindMinMax(T[] array, MyComperator<T> lambda){
		if(null == array || 0 == array.length) {
			return null;
		}
		
		if(1 == array.length) {
			return of(array[0], array[0]);
		}
		
		halfSortArray(array, lambda);

		return of(FindMinInRange(array, 0, array.length/2, lambda),
				  FindMaxInRange(array, array.length/2, array.length - 1, lambda));
	}
	
		public static<T> Pair<T,T> minMax(T[] array, Comparator<T> comp){		
		MyComperator<T> lambda = (T t1, T t2) -> {
			return comp.compare(t1, t2);
		};
		
		return genericFindMinMax(array, lambda);
	}
		
	public static<T extends Comparable<T>> Pair<T,T> minMax(T[] array){
		MyComperator<T> lambda = (T t1, T t2) -> {
			return t1.compareTo(t2);
		};
		
		return genericFindMinMax(array, lambda);
	}
	
/******************************************************************************/
	@Override
	public T getKey() {
		return first;
	};
	@Override
	public U getValue(){
		return second;
	};
	@Override
	public int hashCode() {
	    return Objects.hash(first);
	}
	@Override
	public boolean equals(Object o) {
		if (null == o || !(o instanceof Pair<?, ?>)) {
			return false;
		}
		
		@SuppressWarnings("unchecked")						//manual type check
		Pair<T, U> pair = (Pair<T, U>) o;
		if(!(pair.first.getClass().equals(first.getClass())
		  && pair.second.getClass().equals(second.getClass()))) {
				return false;
		}

		return pair.first.equals(first) && pair.second.equals(second);
	}
	@Override
	public String toString() {
		return "Key: " + first.toString() + " ,Value: " + second.toString();
	}
	@Override
	public U setValue(U value) {
		U prevSecond = second;
		second = value;
		
		return prevSecond;
	}
}