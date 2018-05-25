#include "Puzzle/PuzzleSolver.hpp"
#include "Puzzle/PuzzleHeuristic1.hpp"
#include "Utils/Timer.hpp"
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <iterator>
#include <iostream>

void GeneratePath(Node *current, std::map<Node, Node> *parentMap, std::vector<Puzzle::Move> *moves);


/*Comparator method for priority queue based on Fscore - greater->min_heap*/
class CompareFscore {
public:
	bool operator()(const Node& n1,const Node& n2) const
	{
		return n1.fscore > n2.fscore;
	}
};

/* following 3 functions are just for debugging - not part of actual algorithm*/
void PrintFringe(std::priority_queue<Node, std::vector<Node>, CompareFscore> fringe)
{
	std::cout << "\n Printing elements of Fringe: (Digest -> fscore) \n";
	while (!fringe.empty())
	{
		std::cout << fringe.top().digestValue << "->" << fringe.top().fscore << std::endl;
		fringe.pop();
	}
}

void PrintClosedSet(std::map<Node,int> *backwardCost)
{
	std::cout << "\n Printing elements of Closed Set: \n";
	std::map<Node, int>::iterator itr;
	itr = backwardCost->begin();

	while (itr != backwardCost->end())
	{
		std::cout << "\t" << itr->first.digestValue;
		itr++;
	}
}

void PrintParentMap(std::map<Node, Node> *parentMap)
{
	std::cout << "\n Map Size: " << parentMap->size() << std::endl;
	std::map<Node, Node>::iterator it;
	for (it = parentMap->begin(); it != parentMap->end(); it++)
	{
		std::cout << it->first.digestValue  // string (key)
			<< ':'
			<< it->second.digestValue  // string's value 
			<< std::endl;
	}
}
/* end for debugging*/

bool PuzzleSolver::Solve(Puzzle *puzzle,
	PuzzleHeuristic *h,
	Puzzle::Board *board,
	const double tmax,
	std::vector<Puzzle::Move> *moves)
{
	Utils::Timer::Clock aStarClk;
	Utils::Timer::Start(&aStarClk);

	//add your implementation
	std::cout << "\nStarting A*\n";
	std::priority_queue<Node, std::vector<Node>, CompareFscore> fringe; //openList of MinHeap based on F-Score
	std::map<Node,int> backwardCost; //stores the g-score of expaned nodes - closed set
	std::map<Node, Node> parentMap; //child,parent - tracks the route of the solution

	/* Node is the book keeping structure which binds states to scores, moves and digest. All the nodes 
	in the search algorithm are represented by this structure.*/
	//initial state - push to fringe where it will be expanded
	Node initialNode;
	initialNode.state = board;
	initialNode.fscore = 0;

	/* update digestValue - digestValue is a hash which uniquely identifies each state - it is generated with
	the help of GetDigest(Board*) function in Puzzle.cpp based on position of tiles in the board. Once it is stored
	as part of state, comparision each time just takes constant time access.*/

	initialNode.digestValue = puzzle->GetDigest(board);

	backwardCost[initialNode] = 0;
	parentMap[initialNode] = initialNode;

	fringe.push(initialNode);
	
	while ((!fringe.empty()) && (Utils::Timer::Elapsed(&aStarClk) < tmax))
	{
		//get the node from fringe
		Node current = fringe.top();
		//check whether current state is Goal
		if (puzzle->IsSolved(current.state))
		{
			GeneratePath(&current, &parentMap, moves);
			return true;
		}

		//expanding - remove from open list (fringe)
		fringe.pop();
		std::vector<Node> successors;
		PuzzleSolver::GetSuccessors(puzzle, &current,&successors);
		for (Node successor : successors)
		{
			//puzzle->PrintBoardToConsole(successor.state); //debug statement
			//in this puzzle game we are considering each action cost as 1
			int tentativeScore = (backwardCost[current]) + 1;

			//if not in closed list proceed or if in closed list but the new score is lesser 
			//although in n-puzzle case - the second condition won't be true

			if ((backwardCost.find(successor) == backwardCost.end()) || (tentativeScore < backwardCost.at(successor)))
			{
				//update the new scores and add it to open list
				backwardCost[successor] = tentativeScore;
				successor.fscore = tentativeScore + h->Evaluate(puzzle,successor.state);

				fringe.push(successor);
				parentMap[successor] = current;
			}

		}
	}
	
	return false;

}

//Generate all possible moves and store the results, if exists
bool PuzzleSolver::GetSuccessors(Puzzle *puzzle, Node *current, std::vector<Node> *successors)
{

	Puzzle::Board *temp = puzzle->CopyBoard((current->state));
	if (puzzle->MakeMoveLeft(temp))
	{
		Node node;
		node.state = temp;
		node.generatedAction = Puzzle::MOVE_LEFT;
		node.digestValue = puzzle->GetDigest(temp);
		successors->push_back(node);
	}

	temp = puzzle->CopyBoard((current->state));
	if (puzzle->MakeMoveRight(temp))
	{
		Node node;
		node.state = temp;
		node.generatedAction = Puzzle::MOVE_RIGHT;
		node.digestValue = puzzle->GetDigest(temp);
		successors->push_back(node);
	}

	temp = puzzle->CopyBoard((current->state));
	if (puzzle->MakeMoveUp(temp))
	{
		Node node;
		node.state = temp;
		node.generatedAction = Puzzle::MOVE_UP;
		node.digestValue = puzzle->GetDigest(temp);
		successors->push_back(node);	
	}

	temp = puzzle->CopyBoard((current->state));
	if (puzzle->MakeMoveDown(temp))
	{
		Node node;
		node.state = temp;
		node.generatedAction = Puzzle::MOVE_DOWN;
		node.digestValue = puzzle->GetDigest(temp);
		successors->push_back(node);
	}

	return true;
}


//trace back from goal state to the parent and get the route for solution
void GeneratePath(Node *current,std::map<Node,Node> *parentMap, std::vector<Puzzle::Move> *moves)
{
	Node child = *current;
	Node parent = parentMap->at(child);
	//std::cout << "\n Generating Path! Map Size: " << parentMap->size() << std::endl;

	while (parent != child)
	{
		//std::cout << "\nParent -> Child " << parent.digestValue << " -> " << child.digestValue << std::endl;
		//std::cout << "\nAction: " << child.generatedAction << std::endl;
		moves->push_back(child.generatedAction);
		child = parent;
		parent = parentMap->at(parent);
	}
	std::reverse(moves->begin(), moves->end());
}

