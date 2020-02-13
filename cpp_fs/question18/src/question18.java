
public class question18 
{
	class Useful
	{
		public void f() {}
		public void g() {}
	}

	class MoreUseful extends Useful
	{
		public void h() {}
		public void i() {}
		public void j() {}
		public void k() {}
	}

	public static class RTTITest
	{
		public static void main(String[] args)	
		{
			int aaa = 0;
			Useful[] x = {new Useful(), new MoreUseful()};
			x[0].f();
			x[1].g();
			//x[1].i();
			((MoreUseful)x[1]).i();
			((MoreUseful)x[0]).i();
		}
	}
}
