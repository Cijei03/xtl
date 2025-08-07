import re

def __make_context_string(ContextArray : list[str]) -> str:
	ContextString : str = ""
	for c in ContextArray:
		ContextString = "{}/{}".format(ContextString, c)
	return ContextString

class XTNode:
	def __init__(self):
		self.name : str = ""
		self.pairs : dict[str, str] = {}
		self.childs : list[XTNode] = []

def make_stack(file_name : str) -> list[XTNode]:
	context : list[str] = []
	instances_base : dict[str, int] = {}
	root_nodes : list[XTNode] = []
	stack : dict[str, XTNode] = {}

	with open(file=file_name, mode="r") as file_handle:
		for line in file_handle:
			m = re.fullmatch(pattern="(\\s*)(\\<)(\\S*)\\s{0,1}(\\S*)", string=line)
			if not m == None:
				if len(m.group(1)) + 1 > len(context):
					context.append(m.group(3))
				else:
					context = context[0:len(m.group(1)) + 1]
					context[len(m.group(1))] = m.group(3) if not len(m.group(3)) == 0 else "null"
				
				current_context_string = __make_context_string(ContextArray=context)

				if instances_base.get(current_context_string) == None:
					instances_base[current_context_string] = 0
				else:
					instances_base[current_context_string] = instances_base[current_context_string] + 1

				instanced_context_string = "{}@{}".format(current_context_string, instances_base.get(current_context_string))
				if stack.get(instanced_context_string) == None:
					stack[instanced_context_string] = XTNode()
					stack[instanced_context_string].name = context[len(context) - 1]

				if len(context) > 1:
					parent_instance_id = __make_context_string(ContextArray=context[0:len(context) - 1])
					stack[(parent_instance_id + "@{}").format(instances_base[parent_instance_id])].childs.append(stack[instanced_context_string])
				else:
					root_nodes.append(stack[instanced_context_string])
			else:
				m = re.fullmatch(pattern="(\\t*)(\\S+)=\\'(\\S+)\\'\\s{0,1}", string=line)

				if not m == None:
					current_context_string : str = __make_context_string(ContextArray=context[0:len(m.group(1))])
					instanced_context_string : str = (current_context_string + "@{}").format(instances_base[current_context_string])
					stack[instanced_context_string].pairs[m.group(2)] = m.group(3)
	return root_nodes