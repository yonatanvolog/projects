package innerclasses;

public class Main {

	public static void main(String[] args) {
		OuterClass outer = new OuterClass();
		OuterClass.InnerClass inner = outer.new InnerClass();
		
		inner.InnerHi();
		inner.alphaDog.Woof();
		//inner.foo(23);
		
	}
}

class OuterClass {
	public int x = 0;
	private int uniqe = 10;
	
	private void OuterHi() {
		System.out.println("outer hi");
	}
	
	class InnerClass {
		public int x = 1;
		
		
		class Dog {
			private int age = 0;
			
			public void Woof() {
				System.out.println("woof");
			}
		}
		
		Dog alphaDog = new Dog()
		{
			public void Woof() {
				System.out.println("alpha woof" + uniqe);
			}
		};
		
		
		void InnerHi() {
			System.out.println("inner hi");
			System.out.println("outer var from inner " + OuterClass.this.x);

			OuterClass.this.OuterHi();
		}
	
		void foo(int x) {
			System.out.println("x = " + x);
			System.out.println("this.x = " + this.x);
			System.out.println("OuterClass.this.x = " + OuterClass.this.x);
		}
	}
}