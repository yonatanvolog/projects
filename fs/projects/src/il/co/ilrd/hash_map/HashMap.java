package il.co.ilrd.hash_map;

import java.util.*;

import il.co.ilrd.pair.Pair;

public class HashMap<K,V> implements Map<K, V> {
	private int capacity;
	private List<List<Pair<K, V>>> entries;
	
	public HashMap() {
		this(10);
	}
	
	public HashMap(int capacity) {
		this.capacity = capacity;
		
		entries = new ArrayList<List<Pair<K, V>>>();
		for (int i = 0; i < capacity; ++i) {
			entries.add(new ArrayList<Pair<K,V>>());
		}
	}
	
	@Override
	public void clear() {
		for (List<Pair<K, V>> list : entries) {
			list.clear();
		}
	}

	@Override
	public boolean containsKey(Object key) {
		Pair<K, V> pair = getPairInBucketByKey(getBucketByKey(key), key);
		if(null == pair) {
			return false;
		}
		
		return true;
	}

	@Override
	public boolean containsValue(Object value) {
		ValueSet valSet = new ValueSet();
		for (V valueInPair : valSet) {
			if(valueInPair.equals(value)) {
				return true;
			}
		}
		
		return false;
	}

	@Override
	public Set<Map.Entry<K, V>> entrySet() {
		return new EntrySet();
	}
	
	@Override
	public V get(Object key) {
		if(null == key) {
			return null;			
		}

		Pair<K, V> pair = getPairInBucketByKey(getBucketByKey(key), key);
		if (null == pair) {
			return null;		
		}
		
		return pair.getValue();

	}
	
	private List<Pair<K, V>> getBucketByKey(Object key) {
		return entries.get(getBucket(key));
	}
	
	private Pair<K, V> getPairInBucketByKey(List<Pair<K, V>> bucket, Object key) {
		for (Pair<K, V> pair : bucket) {
			if(pair.getKey().equals(key)) {
				return pair;
			}
		}

		return null;
	}

	@Override
	public boolean isEmpty() {
		for (List<Pair<K, V>> list : entries) {
			if(!list.isEmpty()) {
				return false;
			}
		}
		
		return true;
	}

	@Override
	public Set<K> keySet() {
		return new KeySet();
	}
	
	private int getBucket(Object key) {
		return key.hashCode() % capacity;
	}

	@Override
	public V put(K key, V value) {
		
		V prevValue = null;

		List<Pair<K, V>> bucket = entries.get(getBucket(key));
		Pair<K, V> pair = getPairInBucketByKey(bucket, key);
		if (null == pair) {
			bucket.add(Pair.of(key, value));
			
			return null;	
		}
		
		prevValue = pair.getValue();
		pair.setValue(value);
		
		return prevValue;		
	}

	@Override
	public void putAll(Map<? extends K, ? extends V> value) {
		for(Map.Entry<? extends K, ? extends V> entry: value.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}

	@Override
	public V remove(Object key) {
		if(null == key) {
			return null;			
		}

		List<Pair<K, V>> bucket = getBucketByKey(key);
		
		int index = 0;
		for (Pair<K, V> pair : bucket) {
			
			if(pair.getKey().equals(key)) {
				V prevValue = pair.getValue();
				bucket.remove(index);
				
				return prevValue;
			}
			++index;
		}
		
		return null;
	}

	@Override
	public int size() {
		int totalSize = 0;
		
		for (List<Pair<K, V>> list : entries) {
			totalSize += list.size();
		}
		
		return totalSize;
	}

	@Override
	public Collection<V> values() {
		return new ValueSet();
	}
	
	private class ValueSet extends AbstractSet<V> implements Iterator<V>{
		Iterator<Map.Entry<K, V>> entryIter = new EntrySet().iterator();
			 
		@Override
		public Iterator<V> iterator() {
			return new ValueSet();
		}
		
		public boolean hasNext() {
			return entryIter.hasNext();
		}
		
		public V next() {
			return entryIter.next().getValue();
		}

		@Override
		public int size() {
			return size();
		}
	}	
	
	private class EntrySet extends AbstractSet<Map.Entry<K, V>>{

		@Override
		final public Iterator<Map.Entry<K, V>> iterator() {
			return new EntryIterator();
		}

		@Override
		public int size() {
			return size();
		}
		
		private class EntryIterator implements Iterator<Map.Entry<K, V>>{
			Iterator<List<Pair<K, V>>> outer = entries.listIterator();
			Iterator<Pair<K, V>> inner = outer.next().listIterator();
			
			{
				while(!inner.hasNext() && outer.hasNext()) {
					inner = outer.next().listIterator();
				}
			}
			
			@Override
			public boolean hasNext() {
				if(inner.hasNext() || outer.hasNext()) {
					return true;
				}
				
				return false;
			}

			@Override
			public Entry<K, V> next() {
				Pair<K, V> pairToReturn = inner.next();

				while(!inner.hasNext() && outer.hasNext()) {
					inner = outer.next().listIterator();
				}
				return pairToReturn;
			}
			
		}
	}
	
	private class KeySet extends AbstractSet<K> implements Iterator<K>{
		Iterator<Map.Entry<K, V>> entryIter = new EntrySet().iterator();
			 
		@Override
		public Iterator<K> iterator() {
			return new KeySet();
		}
		
		public boolean hasNext() {
			return entryIter.hasNext();
		}
		
		public K next() {
			return entryIter.next().getKey();
		}

		@Override
		public int size() {
			return size();
		}
	}	
}