#include "../include/xtlp.hpp"
#include <set>
#include <fstream>
#include <regex>
#include <format>

namespace XTLP::r100
{
	std::vector<const XTNode*> XTNodesStack::Read(std::string_view FileName)
	{
		std::set<const XTNode*> NodesUnique;
		std::map<std::string, uint64_t> InstancesBase;

		std::ifstream File(FileName.data());
		{
			std::string Cache;
			std::regex ContextR("(\\s*)(\\<)(\\S*)\\s{0,1}(\\S*)");
			std::regex ValueR("(\\s*)([\\S]+)=\\'(\\S+)\\'");

			std::vector<std::string> Context{};

			while (std::getline(File, Cache, '\n'))
			{
				std::smatch MatchResult;
				std::regex_match(Cache, MatchResult, ContextR);

				if (!MatchResult.empty())
				{
					if (MatchResult[1].str().length() > Context.size())
					{
						Context.push_back(MatchResult[3].str());
					}
					else
					{
						Context.resize(MatchResult[1].str().length() + 1);
						Context.back() = MatchResult[3].str();
					}

					auto CurrentContextString = this->MakeContextString(Context);

					if (!InstancesBase.contains(CurrentContextString))
					{
						InstancesBase[CurrentContextString] = 0;
					}
					else
					{
						InstancesBase[CurrentContextString]++;
					}

					auto InstancedContextString = std::format("{}@{}", CurrentContextString, InstancesBase[CurrentContextString]);

					if (this->AllocationStack[InstancedContextString].get() == nullptr)
					{
						this->AllocationStack[InstancedContextString] = std::make_shared<XTNode>();
					}
					this->AllocationStack[InstancedContextString]->Name = (Context.back().empty() ? "null" : Context.back());
					if (Context.size() > 1)
					{
						auto ParentInstanceString = this->MakeContextString(std::span<std::string>(Context.begin(), Context.end() - 1));
						this->AllocationStack[std::format("{}@{}", ParentInstanceString, InstancesBase[ParentInstanceString])]->Childs.push_back(this->AllocationStack[InstancedContextString].get());
					}
					else
					{
						NodesUnique.insert(const_cast<const XTNode*>(this->AllocationStack[InstancedContextString].get()));
					}
				}
				else
				{
					std::regex_match(Cache, MatchResult, ValueR);

					if (!MatchResult.empty())
					{
						auto CurrentContextString = this->MakeContextString(std::span<std::string>(Context.begin(), Context.begin() + MatchResult[1].str().length()));
						auto InstancedContextString = std::format("{}@{}", CurrentContextString, InstancesBase[CurrentContextString]);
						this->AllocationStack[InstancedContextString]->Pairs[MatchResult[2].str()] = MatchResult[3].str();
					}
				}
			}
		}
		return std::vector<const XTNode*>(NodesUnique.begin(), NodesUnique.end());
	}
}