#pragma once
#include <string_view>
#include <map>
#include <vector>
#include <memory>
#include <span>

namespace XTLP::intern
{
	struct XTNode
	{
		std::string Name;
		std::map<std::string, std::string> Pairs;
		std::vector<const XTNode*> Childs;
	};

	class XTNodesStack
	{
	protected:
		std::map<std::string, std::shared_ptr<XTNode>> AllocationStack;

		std::string MakeContextString(std::span<std::string> C);
	public:
		virtual std::vector<const XTNode*> Read(std::string_view FileName);

		XTNodesStack() = default;
		virtual ~XTNodesStack() = default;
	};
}

namespace XTLP::r100
{
	using XTNode = intern::XTNode;

	class XTNodesStack : public intern::XTNodesStack
	{
	public:
		virtual std::vector<const XTNode*> Read(std::string_view FileName) override;
	};
}

namespace XTLP
{
	using namespace XTLP::r100;
}