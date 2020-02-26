class EX5 
{
    public static void main(String[] args) 
	{
		PrintFrame(0);
	}

	public static int PrintFrame(int frame) 
	{
		if (frame == 50) 
		{
		    return 0;
		}
		System.out.println(frame);

		return PrintFrame(frame + 1);
	}
}
