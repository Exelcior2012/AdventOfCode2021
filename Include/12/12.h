
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <tuple>

struct Day12 : public AoC::PuzzleBase<Day12>
{

	struct Data
	{
		struct Node;
		struct NodeLink
		{
			NodeLink(Node* inNode)
				: Link(inNode)
			{}

			Node* Link;
			uint8_t Value = 1;
		};

		struct Node
		{
			Node(std::string_view inName)
				: Name(inName)
			{}

			std::string Name;
			std::vector<NodeLink> Links;
			uint32_t Id;
			bool IsSmall:1;
		};

		std::vector<Node> Nodes;
		Node* StartNode;
		Node* EndNode;
		
		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string firstNode;
			string secondNode;
			vector<tuple<string, string>> intermediate;
			unordered_map<string_view, Node*> nameToNodeMap;
			
			// First just get all the nodes
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

			// Set IDs and map names
			int count = 0;
			for(Node& n : out.Nodes)
			{
				nameToNodeMap[{n.Name.data(), n.Name.size()}] = &n;
				n.Id = count++;
			}

			// Add input links
			for(const auto& link : intermediate)
			{
				Node* first = nameToNodeMap[get<0>(link)];
				Node* second = nameToNodeMap[get<1>(link)];

				first->Links.emplace_back(second);
				second->Links.emplace_back(first);
			}

			// Here we remove all large caves, and adding links out from each
			// small cave to all other links of the large cave to compact the graph
			for(Node& node : out.Nodes)
			{
				for(int i = 0; i < (int)node.Links.size(); ++i)
				{
					Node* outerLink = node.Links[i].Link;
					if(!outerLink->IsSmall)
					{
						// Swap the large cave link to the end and remove it
						std::swap(node.Links[i], node.Links.back());
						--i; // removed one, need to account for on next loop
						node.Links.pop_back();

						for(NodeLink& ln : outerLink->Links)
						{
							auto it = find_if(begin(node.Links), end(node.Links), [&ln](const NodeLink& findLink) { return findLink.Link == ln.Link; });

							// If we have a duplicate link - just increase the value.
							// this will be used later to reduce the number of paths
							// through the graph
							if(it != end(node.Links))
							{
								(*it).Value++;
							}
							else if(ln.Link->Name != "start")
							{
								// Unique link, add it to the list
								node.Links.emplace_back(ln.Link);
							}
						}
					}
				}
			}

			// Get the start and end nodes
			out.StartNode = nameToNodeMap["start"];
			out.EndNode = nameToNodeMap["end"];

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

	template<bool allowMultiple = false>
	static void CountPaths(const Data::Node* start, const Data::Node* end, bool& blockMultiple, int32_t& numPaths, std::vector<int>& visitList, int32_t value = 1)
	{
		// Mark this node
		++visitList[start->Id];
		
		if constexpr(allowMultiple)
		{
			// If we hit this node more than once, stop further multi-visits
			if(visitList[start->Id] == 2)
			{
				blockMultiple = true;
			}
		}

		if(start == end)
		{
			numPaths += value;
		}
		else
		{
			for(const Data::NodeLink& adj : start->Links)
			{
				const int visits = visitList[adj.Link->Id];
				// If we've visited the path, don't proceed - unless we're allowing
				// nodes to have more than one visit and we're not blocking multi-visits
				if(visits == 0 || (visits == 1 && !blockMultiple))
				{
					// Note adj.Value * value, this cuts down on the paths we need to account for
					// as we can just add that to the path number once we hit the end.
					CountPaths<allowMultiple>(adj.Link, end, blockMultiple, numPaths, visitList, adj.Value * value);
				}
			}
		}

		// Other paths can proceed through this node again.
		// Release the block flag so multi-visits can happen on later paths
		if constexpr(allowMultiple)
		{
			if(visitList[start->Id] == 2)
			{
				blockMultiple = false;
			}
		}

		--visitList[start->Id];
	}

	static int64_t Part1(const Data& data)
	{
		int32_t pathCount = 0;
		std::vector<int> visitList(data.Nodes.size(), 0);

		// No multi-visits
		bool blocked = true;

		CountPaths(data.StartNode, data.EndNode, blocked, pathCount, visitList);

		return pathCount;
	}

	static int64_t Part2(const Data& data)
	{
		int32_t pathCount = 0;
		bool blocked = false;
		std::vector<int> visitList(data.Nodes.size(), 0);

		// Make sure we never attempt to go into the start node after the first call
		visitList[data.StartNode->Id] = 2;
		
		CountPaths<true>(data.StartNode, data.EndNode, blocked, pathCount, visitList);

		return pathCount;
	}
};

