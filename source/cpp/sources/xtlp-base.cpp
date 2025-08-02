#include "../include/xtlp.hpp"
#include <format>

namespace XTLP::intern
{
	std::string XTNodesStack::MakeContextString(std::span<std::string> C)
	{
		std::string ContextString;
		for (auto c : C)
		{
			ContextString = std::format("{}/{}", ContextString, c);
		}
		return ContextString;
	}

	std::vector<const XTNode*> XTNodesStack::Read(std::string_view FileName)
	{
		return {};
	}
}