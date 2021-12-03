
#include "PuzzleFramework.h"

#define DOCOPT_HEADER_ONLY
#include "docopt.h"

// Puzzles
#include "01/01.h"
#include "02/02.h"
#include "03/03.h"

static std::map<std::string, docopt::value> GArgs;

static constexpr char usage[] =
R"(AdventOfCode2021

	Usage:
		AdventOfCode2021 [options] [<puzzlename> ...]

	Options:
		-h --help                           Show this message.
		-r --redact                         Redact output of answers, showing only timings
		-i <iters>, --iterations <iters>    Number of iterations to run for timing [default: 256]
)";

template<AoC::CPuzzle Puzzle_t>
void ConditionalRunPuzzle()
{
	using namespace std;

	const int32_t numIters = GArgs["--iterations"].asLong();
	const bool redact = GArgs["--redact"].asBool();
	const vector<string>& puzzleNames = GArgs["<puzzlename>"].asStringList();

	auto nameIter = find(begin(puzzleNames), end(puzzleNames), Puzzle_t::GetName());

	if(puzzleNames.size() == 0 || nameIter != end(puzzleNames))
	{
		AoC::Run<Puzzle_t>(numIters, redact);
	}
}

int main(int ArgCount, char** Args)
{
	GArgs = docopt::docopt(usage, {Args + 1, Args + ArgCount}, true, "AoC2021");

	ConditionalRunPuzzle<Day1>();
	ConditionalRunPuzzle<Day2>();
	ConditionalRunPuzzle<Day3>();

	return 0;
}
