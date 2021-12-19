
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <charconv>

template<typename T, bool AsChar = false>
struct TBinaryTreeNode
{
	enum class EDirection
	{
		Left,
		Right
	};

	using Elem_t = T;

	TBinaryTreeNode(const Elem_t& init, TBinaryTreeNode<T>* parent = nullptr)
		: Element(init)
		, Parent(parent)
	{
	}

	~TBinaryTreeNode()
	{
		delete Children[0];
		delete Children[1];
	}

	Elem_t Element = {};

	TBinaryTreeNode<T>* Parent = nullptr;
	TBinaryTreeNode<T>* Children[2] = {nullptr, nullptr};

	TBinaryTreeNode<T>* WalkToLeaf(EDirection dir)
	{
		const int dirIndex = dir == EDirection::Left ? 0 : 1;
		const int otherDirIndex = 1 - dirIndex;
		
		assert(IsValue());

		if(!(Parent->Children[dirIndex] == this)) __debugbreak();
		assert(Parent->Children[dirIndex] == this); 

		TBinaryTreeNode<T>* at = this;

		if(at->Children[dirIndex])
		{
			at = at->Children[dirIndex];
			while(at->Children[otherDirIndex])
			{
				at = at->Children[otherDirIndex];
			}
			return at;
		}
		else
		{
			while(at->Parent && at->Parent->Children[dirIndex] == at)
			{
				// walk up until we are a right child
				at = at->Parent;
			}

			// walk left
			if(!at->Parent) return nullptr;

			at = at->Parent->Children[dirIndex];
			while(at->Children[otherDirIndex])
			{
				// walk right until we hit the end
				at = at->Children[otherDirIndex];
			}

			return at;
		}
	}

	TBinaryTreeNode<T>* PreviousLeaf()
	{
		return WalkToLeaf(EDirection::Left);
	}

	TBinaryTreeNode<T>* NextLeaf()
	{
		return WalkToLeaf(EDirection::Right);
	}

	bool IsValue() const
	{
		return Children[0] == nullptr;
	}

	bool IsValuePair() const
	{
		return (Children[0] && !Children[0]->Children[0]) &&
			(Children[1] && !Children[1]->Children[1]);
	}

	friend void PrintNode(std::ostream& out, const TBinaryTreeNode<T>& tree)
	{
		if(tree.Children[0] && tree.Children[1])
		{
			out << '[';
			PrintNode(out, *tree.Children[0]);
			out << ',';
			PrintNode(out, *tree.Children[1]);
			out << ']';
		}
		else
		{
			if constexpr(std::is_integral_v<T> && !AsChar)
			{
				// ouput treats u16+u8 as char types and mangles the output
				// so instead we widen all integer types to their largest
				// width for output. This means chars also print as integers
				// unless AsChar == true
				out << static_cast<int64_t>(tree.Element);
			}
			else
			{
				out << tree.Element;
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const TBinaryTreeNode<T>& tree)
	{
		PrintNode(out, tree);
		return out;
	}
};

bool ExplodeNode(TBinaryTreeNode<int8_t>* node, int depth)
{
	if(!node)
	{
		return false;
	}

	if(node->IsValuePair() && depth > 4)
	{
		// explode
		TBinaryTreeNode<int8_t>* prevLeaf = node->Children[0]->PreviousLeaf();
		TBinaryTreeNode<int8_t>* nextLeaf = node->Children[1]->NextLeaf();

		if(prevLeaf)
		{
			assert(prevLeaf->IsValue());
			prevLeaf->Element += node->Children[0]->Element;
		}

		if(nextLeaf)
		{
			assert(nextLeaf->IsValue());
			nextLeaf->Element += node->Children[1]->Element;
		}

		node->Element = 0;
		delete node->Children[0];
		delete node->Children[1];
		node->Children[0] = nullptr;
		node->Children[1] = nullptr;

		return true;
	}
	
	if(ExplodeNode(node->Children[0], depth + 1)) return true;
	if(ExplodeNode(node->Children[1], depth + 1)) return true;

	return false;
}

bool SplitNode(TBinaryTreeNode<int8_t>* node)
{
	if(!node)
	{
		return false;
	}

	if(node->IsValue() && node->Element >= 10)
	{
		// split
		node->Children[0] = new TBinaryTreeNode<int8_t>(node->Element / 2, node);
		node->Children[1] = new TBinaryTreeNode<int8_t>((node->Element + 1) / 2, node);
		node->Element = -1;
		return true;
	}
	
	if(SplitNode(node->Children[0])) return true;
	if(SplitNode(node->Children[1])) return true;

	return false;
}

bool ExplodeNode_WithLog(TBinaryTreeNode<int8_t>* node, int depth)
{
	const bool result = ExplodeNode(node, depth);
	if(result) { Util::Log("\tExplode: ", *node); }
	return result;
}

bool SplitNode_WithLog(TBinaryTreeNode<int8_t>* node)
{
	const bool result = SplitNode(node);
	if(result) { Util::Log("\tSplit: ", *node); }
	return result;
}

void ReduceTree(TBinaryTreeNode<int8_t>* node)
{
	do 
	{
		while(ExplodeNode(node, 1)) {}
	} while (SplitNode(node));
}

int64_t GetMagnitude(TBinaryTreeNode<int8_t>* node)
{
	assert(node);
	if(node->IsValue())
	{
		return node->Element;
	}
	else
	{
		return  3 * GetMagnitude(node->Children[0]) + 2 * GetMagnitude(node->Children[1]);
	}
}

struct Day18 : public AoC::PuzzleBase<Day18>
{
	struct Data
	{
		using Tree_t = TBinaryTreeNode<int8_t>;
		using Treeptr = std::unique_ptr<Tree_t>;
		std::vector<Tree_t*> Trees;

		static size_t FindPairEnd(std::string_view str)
		{
			assert(str[0] == '[');
			int depth = 1;
			const char* curr = str.data();
			++curr;
			while(depth > 0)
			{
				if(*curr == '[') ++depth;
				else if(*curr == ']') --depth;
				++curr;
			}
			return std::distance(str.data(), curr - 1);
		}

		static void ParsePair(std::string_view str, Tree_t* node)
		{
			node->Children[0] = new Tree_t(-1, node);
			node->Children[1] = new Tree_t(-1, node);

			size_t outerEnd = FindPairEnd(str);
			std::string_view unwrapped = str.substr(1, outerEnd - 1);

			if(unwrapped[0] == '[')
			{
				size_t pairEnd = FindPairEnd(unwrapped);
				ParsePair(unwrapped.substr(0, pairEnd + 1), node->Children[0]);
				unwrapped = unwrapped.substr(pairEnd + 1);
			}
			else
			{
				int value = -1;
				size_t numEnd = unwrapped.find_first_of(',');
				std::string_view numView = unwrapped.substr(0, numEnd);
				std::from_chars(numView.data(), numView.data() + numView.size(), value);
				node->Children[0]->Element = static_cast<int8_t>(value);
				unwrapped = unwrapped.substr(numEnd);
			}

			unwrapped.remove_prefix(1); // skip the comma

			if(unwrapped[0] == '[')
			{
				size_t pairEnd = FindPairEnd(unwrapped);
				ParsePair(unwrapped.substr(0, pairEnd + 1), node->Children[1]);
			}
			else
			{
				int value = -1;
				std::from_chars(unwrapped.data(), unwrapped.data() + unwrapped.size(), value);
				node->Children[1]->Element = static_cast<int8_t>(value);
			}
		}

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			while(in.good())
			{
				getline(in, line);
				if(line.empty()) continue;

				out.Trees.emplace_back(new Tree_t(static_cast<typename Tree_t::Elem_t>(-1)));
				ParsePair(line, out.Trees.back());
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/18/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 18: Snailfish";
	}

	static void CopyTreeInner(Data::Tree_t* source, Data::Tree_t* destination)
	{
		destination->Element = source->Element;
		
		if(source->Children[0])
		{
			destination->Children[0] = new TBinaryTreeNode<int8_t>(-1);
			destination->Children[0]->Parent = destination;
			CopyTreeInner(source->Children[0], destination->Children[0]);
		}

		if(source->Children[1])
		{
			destination->Children[1] = new TBinaryTreeNode<int8_t>(-1);
			destination->Children[1]->Parent = destination;
			CopyTreeInner(source->Children[1], destination->Children[1]);
		}
	}

	static Data::Tree_t* CopyTree(Data::Tree_t* source)
	{
		Data::Tree_t* newRoot = new Data::Tree_t(-1);
		CopyTreeInner(source, newRoot);
		return newRoot;
	}

	static int64_t Part1(const Data& data)
	{
		Data copy = data;
		
		Data::Tree_t* last = CopyTree(copy.Trees[0]);
		Data::Tree_t* root = last;
		
		const size_t numSums = copy.Trees.size();
		for(size_t i = 1; i < numSums; ++i)
		{
			root = new Data::Tree_t(-1);
			root->Children[0] = last;
			root->Children[1] = CopyTree(copy.Trees[i]);
			root->Children[0]->Parent = root;
			root->Children[1]->Parent = root;
			last = root;
			ReduceTree(root);
		}
		int64_t mag = GetMagnitude(root);
		delete root;

		return mag;
	}

	static int64_t Part2(const Data& data)
	{
		Data copy = data;

		// Fish math is non-commutative, so need n^2 checks
		int64_t largest = 0;
		const size_t numSums = data.Trees.size();
		for(size_t i = 0; i < numSums; ++i)
		{
			for(size_t j = 0; j < numSums; ++j)
			{
				if(j == i) continue;

				Data::Tree_t* a = CopyTree(copy.Trees[i]);
				Data::Tree_t* b = CopyTree(copy.Trees[j]);

				Data::Tree_t* root = new Data::Tree_t(-1);
				root->Children[0] = a;
				root->Children[1] = b;
				root->Children[0]->Parent = root;
				root->Children[1]->Parent = root;
				ReduceTree(root);
				largest = std::max(largest, GetMagnitude(root));

				delete root;
			}
		}

		return largest;
	}
};
