
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <tuple>

struct Day12 : public AoC::PuzzleBase<Day12>
{

	struct Data
	{
		struct Node
		{
			Node(std::string_view inName)
				: Name(inName)
			{}

			std::string Name;
			std::vector<Node*> Links;
			uint32_t Id;
			bool IsSmall:1;
		};

		std::vector<Node> Nodes;
		Node* StartNode;
		Node* EndNode;
		std::unordered_map<std::string_view, Node*> NameToNodeMap;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string firstNode;
			string secondNode;
			vector<tuple<string, string>> intermediate;
			while(in.good())
			{
 				getline(in, firstNode, '-');
				getline(in, secondNode);

				auto AppendNode = [&out](string_view name)
				{
					auto it = find_if(begin(out.Nodes), end(out.Nodes), [&name](const Node& n)
					{
						return name == n.Name;
					});

					if(it == end(out.Nodes))
					{
						out.Nodes.emplace_back(name);

						out.Nodes.back().IsSmall = true;
						for(char c : name)
						{
							if(c < 'a')
							{
								out.Nodes.back().IsSmall = false;
								break;
							}
						}
					}
				};

				AppendNode(firstNode);
				AppendNode(secondNode);
				intermediate.emplace_back(make_tuple(firstNode, secondNode));
			}

			int count = 0;
			for(Node& n : out.Nodes)
			{
				out.NameToNodeMap[{n.Name.data(), n.Name.size()}] = &n;
				n.Id = count++;
			}

			for(const auto& link : intermediate)
			{
				Node* first = out.NameToNodeMap[get<0>(link)];
				Node* second = out.NameToNodeMap[get<1>(link)];
				first->Links.push_back(second);
				second->Links.push_back(first);

				if(first->Name == "start") out.StartNode = first;
				if(first->Name == "end") out.EndNode = first;
				if(second->Name == "start") out.StartNode = second;
				if(second->Name == "end") out.EndNode = second;
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/12/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 12: Passage Pathing";
	}

	template<int SmallVisitCount = 1, typename Visitor>
	static void VisitPaths(const Data::Node* start, const Data::Node* end, bool& blocked, std::vector<int>& visitList, const Visitor& v)
	{
		// If we need to stop visiting this node, flag it
		if(start->IsSmall)
		{
			++visitList[start->Id];
			
			// if we hit the small limit, allow no further multi-visits
			if(visitList[start->Id] == SmallVisitCount)
			{
				blocked = true;
			}
		}

		if(start == end)
		{
			// At the end, call the visitor
			v();
		}
		else
		{
			for(const Data::Node* adj : start->Links)
			{
				const int visits = visitList[adj->Id];
				// If we've visited the path, don't proceed - unless we're allowing
				// nodes to have more than one visit and we're not blocking multi-visits
				if(visits == 0 || (visits < SmallVisitCount && !blocked))
				{
					VisitPaths<SmallVisitCount>(adj, end, blocked, visitList, v);
				}
			}
		}

		// Other paths can proceed through this node again.
		if(start->IsSmall)
		{
			// Release the block flag so multi-visits can happen on later paths
			if(visitList[start->Id] == SmallVisitCount)
			{
				blocked = false;
			}

			--visitList[start->Id];
		}
	}

	static int64_t Part1(const Data& data)
	{
		int pathCount = 0;
		std::vector<int> visitList(data.Nodes.size(), 0);

		// No multi-visits
		bool blocked = true;

		VisitPaths(data.StartNode, data.EndNode, blocked, visitList, [&pathCount]() { pathCount++; });

		return pathCount;
	}

	static int64_t Part2(const Data& data)
	{
		int pathCount = 0;
		bool blocked = false;
		std::vector<int> visitList(data.Nodes.size(), 0);

		// Make sure we never attempt to go into the start node after the first call
		visitList[data.StartNode->Id] = 2;
		
		VisitPaths<2>(data.StartNode, data.EndNode, blocked, visitList, [&pathCount]() {pathCount++; });

		return pathCount;
	}
};

