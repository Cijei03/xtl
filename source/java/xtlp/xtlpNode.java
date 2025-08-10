package xtlp;

import java.util.ArrayList;
import java.util.TreeMap;

public class xtlpNode
{
	public String Name;
	public TreeMap<String, String> Pairs;
	public ArrayList<xtlpNode> Childs;

	public xtlpNode()
	{
		this.Pairs = new TreeMap<String, String>();
		this.Childs = new ArrayList<xtlpNode>();
	}
}
