package xtlp;

import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.TreeMap;
import java.nio.file.Files;
import java.util.stream.Stream;
import java.util.regex.Matcher;
import java.nio.file.Paths;
import java.io.IOException;

public class xtlpRoot
{
	private ArrayList<String> ManageContext(Matcher m, ArrayList<String> Context)
	{
		if (m.group(1).length() > Context.size())
		{
			Context.add(m.group(3));
		}
		else
		{
			Context = new ArrayList<String>(Context.subList(0, m.group(1).length()));
			Context.add(m.group(3));
		}
		return Context;
	}
	private String MakeContextString(Object[] C) 
	{
		var Cache = new String();
		for (var c : C)
		{
			Cache = String.format("%s/%s", Cache, c.toString());
		}
		return Cache;
	}
	private void ManageNodeScope(TreeMap<String, xtlpNode> NodesStack, ArrayList<String> Context, xtlpNode NewNode)
	{
		if (Context.size() > 1)
		{
			var ParentInstanceString = this.MakeContextString(Context.subList(0, Context.size() - 1).toArray());
			NodesStack.get(ParentInstanceString).Childs.add(NewNode);
		}
		else
		{
			this.Nodes.add(NewNode);
		}
	}
	public ArrayList<xtlpNode> Nodes = new ArrayList<xtlpNode>();

	public xtlpRoot(final String FileName) throws IOException
	{
		// Open file.
		Object[] LinesList = null;
		{
			Stream <String> FileStream = Files.lines(Paths.get(FileName));
			LinesList = FileStream.toArray();
			FileStream.close();
		}

		// Compile regexes.
		Pattern ContextPattern = Pattern.compile("(\\s*)(\\<)(\\S*)\\s{0,1}(\\S*)");
		Pattern PairPattern = Pattern.compile("(\\s*)([\\S]+)=\\'(\\S+)\\'");
		
		// Preallocate necessary objects.
		var Context = new ArrayList<String>();
		var NodesStack = new TreeMap<String, xtlpNode>();		

		for (var Line : LinesList)
		{
			Matcher m = ContextPattern.matcher(Line.toString());

			if (m.find())
			{
				Context = this.ManageContext(m, Context);

				var CurrentContextString = this.MakeContextString(Context.toArray());

				xtlpNode Node = new xtlpNode();
				Node.Name = Context.getLast().length() > 0 ? Context.getLast() : "null";
				NodesStack.put(CurrentContextString, Node);

				this.ManageNodeScope(NodesStack, Context, Node);
			}
			else
			{
				Matcher m_pair = PairPattern.matcher(Line.toString());

				if (m_pair.find())
				{
					var CurrentContextString = this.MakeContextString(Context.subList(0, m_pair.group(1).length()).toArray());
					NodesStack.get(CurrentContextString).Pairs.put(m_pair.group(2), m_pair.group(3));
				}
			}
		}
	}
}
