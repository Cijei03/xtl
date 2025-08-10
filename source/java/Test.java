import java.util.ArrayList;

import xtlp.xtlpNode;
import xtlp.xtlpRoot;

public class Test
{
	private static void DisplayContent(ArrayList<xtlpNode> Nodes)
	{
		for (var N : Nodes)
		{
			System.out.println(String.format("Node name: %s", N.Name));

			for (var P : N.Pairs.entrySet())
			{
				System.out.println(String.format("Pair: %s = %s", P.getKey(), P.getValue()));
			}

			DisplayContent(N.Childs);
		}
	}
	public static void main(String[] args)
	{
		try
		{
			xtlpRoot Root = new xtlpRoot("cfg.xtl");
			Test.DisplayContent(Root.Nodes);
		}
		catch(java.io.IOException e)
		{
			System.err.println("IOERROR");
		}
	}
}
