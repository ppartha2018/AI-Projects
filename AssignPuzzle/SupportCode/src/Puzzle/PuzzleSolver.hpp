#ifndef PUZZLE_SOLVER_HPP_
#define PUZZLE_SOLVER_HPP_

#include "Puzzle/Puzzle.hpp"
#include "Puzzle/PuzzleHeuristic.hpp"
#include <vector>

class PuzzleSolver
{
public:
    PuzzleSolver(void)
    {
    }
    
    virtual ~PuzzleSolver(void)
    {
    }

    //you should implement this function   
    //board is the start configuration    
    //return true if solution found; false otherwise
    //add the solution moves to the moves vector
    virtual bool Solve(Puzzle *puzzle, 
		       PuzzleHeuristic *h, 
		       Puzzle::Board *board, 
		       const double tmax, 
		       std::vector<Puzzle::Move> *moves);

	virtual bool GetSuccessors(Puzzle *puzzle, Node *current, std::vector<Node> *successors);

};



#endif
    
