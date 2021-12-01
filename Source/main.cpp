
#include "PuzzleFramework.h"

#define DOCOPT_HEADER_ONLY
#include "docopt.h"

// Puzzles
#include "01/01.h"

static std::map<std::string, docopt::value> GArgs;

static constexpr char usage[] =
R"(AdventOfCode2021

	Usage:
		AdventOfCode2021 [options] [<puzzlename> ...]

	Options:
		-h --help                           Show this message.
		-i <iters>, --iterations <iters>    Number of iterations to run for timing [default: 256]
)";

template<AoC::CPuzzle Puzzle_t>
void ConditionalRunPuzzle()
{
	using namespace std;

	const int32_t numIters = GArgs["--iterations"].asLong();
	const std::vector<std::string>& puzzleNames = GArgs["<puzzlename>"].asStringList();

	auto nameIter = find(begin(puzzleNames), end(puzzleNames), Puzzle_t::GetName());

	if(puzzleNames.size() == 0 || nameIter != end(puzzleNames))
	{
		AoC::Run<Puzzle_t>(numIters);
	}
}

int main(int ArgCount, char** Args)
{
	using namespace std;

	GArgs = docopt::docopt(usage, {Args + 1, Args + ArgCount}, true, "AoC2021");

	ConditionalRunPuzzle<Day1>();

	return 0;
}