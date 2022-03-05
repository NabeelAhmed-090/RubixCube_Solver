#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

/*
	Helper funtions for printing
*/
void gotoxy(int x, int y)
{
	/*
		helper function to set the coordinates for pritning
	*/
	COORD coordinates;
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}
void setOutputColor(char c) {
	/*
		helper function to set the output console color
	*/
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int num = c == 'r' ? 4 :
		c == 'g' ? 2 :
		c == 'b' ? 1 :
		c == 'y' ? 14 :
		c == 'o' ? 6 : 15;
	SetConsoleTextAttribute(hConsole, num);
}

/*
	RubixSide class
*/
class RubixSide {
	/*
		RubixSide class
		Contains 2D array of characters
	*/
	char** side;
	void assignMemory() {
		/*
			helper function to assign memory to the 2D array
		*/
		this->side = new char* [3];
		for (int i = 0; i < 3; i++) {
			this->side[i] = new char[3];
		}
	}
public:
	RubixSide() {
		side = nullptr;
	}
	RubixSide(char** side) {
		this->side = nullptr;
		setSide(side);
	}
	void setSide(char** side) {
		/*
			function to set the 2D array
		*/
		if (this->side != nullptr) {
			for (int i = 0; i < 3; i++)
				delete[](this->side)[i];
			delete[](this->side);
			this->side = nullptr;
		}
		assignMemory();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				this->side[i][j] = side[i][j];
	}
	void printSide(int x, int y, string sideName) {
		/*
			function to print the side on coordinates x, y
		*/
		gotoxy(x, y - 2);
		cout << "side : " << sideName;
		gotoxy(x, y);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				setOutputColor(side[i][j]);
				cout << side[i][j] << "   ";
			}
			y = y + 1;
			gotoxy(x, y);
		}
		setOutputColor('w');
	}
	char** getSide() {
		/*
			function to get the 2D array
		*/
		return side;
	}
	void updateRow(int rowNum, char* row) {
		/*
			update a specific row in the 2D array
		*/
		for (int i = 0; i < 3; i++)
			side[rowNum][i] = row[i];
	}
	void updateCol(int colNum, char* col) {
		/*
			update a specific col in the 2D array
		*/
		for (int i = 0; i < 3; i++)
			side[i][colNum] = col[i];
	}
	bool sameColor() {
		/*
			function that checks if all the indexes have same color or not
		*/
		char color = side[0][0];
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (color != side[i][j])
					return false;

		return true;
	}
	bool operator==(const RubixSide& side) {
		/*
			Operator overloaded to check if the 2 sides are same or not
		*/
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (this->side[i][j] != side.side[i][j])
					return false;
		return true;
	}
};

/*
	RubixCube class
*/
class RubixCube {
	/*
		Comprised of 6 sides. Each Side is represented by a RubixSide pointer.
	*/
	RubixSide* front;
	RubixSide* back;
	RubixSide* right;
	RubixSide* left;
	RubixSide* top;
	RubixSide* bottom;
	void assignMemory() {
		/*
			helper function to assign memory to the RubixCube
		*/
		front = new RubixSide();
		back = new RubixSide();
		right = new RubixSide();
		left = new RubixSide();
		top = new RubixSide();
		bottom = new RubixSide();
	}
public:
	RubixCube() {
		front = back = right = left = top = bottom = nullptr;
	}
	RubixCube(RubixSide* front, RubixSide* back,
		RubixSide* right, RubixSide* left,
		RubixSide* top, RubixSide* bottom) {
		assignMemory();
		this->front->setSide(front->getSide());
		this->back->setSide(back->getSide());
		this->right->setSide(right->getSide());
		this->left->setSide(left->getSide());
		this->top->setSide(top->getSide());
		this->bottom->setSide(bottom->getSide());
	}
	RubixCube(const RubixCube& cube) {
		assignMemory();
		this->front->setSide(cube.front->getSide());
		this->back->setSide(cube.back->getSide());
		this->right->setSide(cube.right->getSide());
		this->left->setSide(cube.left->getSide());
		this->top->setSide(cube.top->getSide());
		this->bottom->setSide(cube.bottom->getSide());
	}
	bool operator==(const RubixCube& cube) {
		/*
			Operator overloaded to see if 2 Rubix Cubes are same or not
		*/
		return (*front == *cube.front) && (*back == *cube.back) &&
			(*right == *cube.right) && (*left == *cube.left) &&
			(*top == *cube.top) && (*bottom == *cube.bottom);
	}
	void printRubixCube(string move = "") {
		/*
			Function to print the Rubix Cube
		*/
		gotoxy(0, 0);
		cout << "                   ";
		gotoxy(0, 0);
		cout << move << endl;
		front->printSide(10, 4, "front");
		back->printSide(30, 4, "back");
		right->printSide(10, 10, "right");
		left->printSide(30, 10, "left");
		top->printSide(10, 16, "top");
		bottom->printSide(30, 16, "bottom");
	}
	bool allSidesFixed() {
		/*
			Function that checks if the goal state is reached or not
		*/
		return (front->sameColor() && back->sameColor()
			&& right->sameColor() && left->sameColor()
			&& top->sameColor() && bottom->sameColor());
	}
	/*
		Functions to rotate the cube
	*/
	void moveFront() {
		/*
			Function that rotates the front side clockwise
		*/
		char rightSideLeftCol[3], bottomSideTopRoW[3], leftSideRightCol[3], topSideBottomRow[3];
		char frontSide[3][3];
		for (int i = 0; i < 3; i++) {
			rightSideLeftCol[i] = right->getSide()[2 - i][0];
			bottomSideTopRoW[i] = bottom->getSide()[0][i];
			leftSideRightCol[i] = left->getSide()[2 - i][2];
			topSideBottomRow[i] = top->getSide()[2][i];
		}
		right->updateCol(0, topSideBottomRow);
		bottom->updateRow(0, rightSideLeftCol);
		left->updateCol(2, bottomSideTopRoW);
		top->updateRow(2, leftSideRightCol);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				frontSide[i][j] = front->getSide()[2 - i][j];

		front->updateCol(0, frontSide[0]);
		front->updateCol(1, frontSide[1]);
		front->updateCol(2, frontSide[2]);
	}
	void moveBack() {
		/*
			Function that rotates the back side anti clockwise. From the front the move is viewed as clockwise.
		*/
		char rightSideRightCol[3], bottomSideBottomRoW[3], leftSideLeftCol[3], topSideTopRow[3];
		char backSide[3][3];
		for (int i = 0; i < 3; i++) {
			rightSideRightCol[i] = right->getSide()[2 - i][2];
			bottomSideBottomRoW[i] = bottom->getSide()[2][i];
			leftSideLeftCol[i] = left->getSide()[2 - i][0];
			topSideTopRow[i] = top->getSide()[0][i];
		}
		right->updateCol(2, topSideTopRow);
		bottom->updateRow(2, rightSideRightCol);
		left->updateCol(0, bottomSideBottomRoW);
		top->updateRow(0, leftSideLeftCol);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				backSide[i][j] = back->getSide()[i][2 - j];

		back->updateCol(0, backSide[0]);
		back->updateCol(1, backSide[1]);
		back->updateCol(2, backSide[2]);
	}
	void moveRight() {
		/*
			Function that rotates the cube right side clockwise
		*/
		char frontSideRightCol[3], backSideLeftCol[3], topSideRightCol[3], bottomSideRightCol[3];
		char rightSide[3][3];
		for (int i = 0; i < 3; i++) {
			frontSideRightCol[i] = front->getSide()[i][2];
			backSideLeftCol[i] = back->getSide()[2 - i][0];
			topSideRightCol[i] = top->getSide()[2 - i][2];
			bottomSideRightCol[i] = bottom->getSide()[i][2];
		}
		front->updateCol(2, bottomSideRightCol);
		back->updateCol(0, topSideRightCol);
		top->updateCol(2, frontSideRightCol);
		bottom->updateCol(2, backSideLeftCol);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				rightSide[i][j] = right->getSide()[i][j];

		right->updateCol(0, rightSide[2]);
		right->updateCol(1, rightSide[1]);
		right->updateCol(2, rightSide[0]);
	}
	void moveLeft() {
		/*
			Function that rotates the cube left side clockwise
		*/
		char frontSideLeftCol[3], backSideRightCol[3], topSideLeftCol[3], bottomSideLeftCol[3];
		char leftSide[3][3];
		for (int i = 0; i < 3; i++) {
			frontSideLeftCol[i] = front->getSide()[i][0];
			backSideRightCol[i] = back->getSide()[2 - i][2];
			topSideLeftCol[i] = top->getSide()[2 - i][0];
			bottomSideLeftCol[i] = bottom->getSide()[i][0];
		}
		front->updateCol(0, bottomSideLeftCol);
		back->updateCol(2, topSideLeftCol);
		top->updateCol(0, frontSideLeftCol);
		bottom->updateCol(0, backSideRightCol);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				leftSide[i][j] = left->getSide()[j][i];

		left->updateRow(0, leftSide[2]);
		left->updateRow(1, leftSide[1]);
		left->updateRow(2, leftSide[0]);
	}
	void moveTop() {
		/*
			Function that rotates the top clockwise
		*/
		char frontSideTopRow[3], backSideTopRow[3], LeftSideTopRow[3], RightSideTopRow[3];
		char topSide[3][3];
		for (int i = 0; i < 3; i++) {
			frontSideTopRow[i] = front->getSide()[0][i];
			backSideTopRow[i] = back->getSide()[0][i];
			LeftSideTopRow[i] = left->getSide()[0][i];
			RightSideTopRow[i] = right->getSide()[0][i];
		}
		front->updateRow(0, LeftSideTopRow);
		back->updateRow(0, RightSideTopRow);
		right->updateRow(0, frontSideTopRow);
		left->updateRow(0, backSideTopRow);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				topSide[i][j] = top->getSide()[j][i];

		top->updateRow(2, topSide[0]);
		top->updateRow(1, topSide[1]);
		top->updateRow(0, topSide[2]);
	}
	void moveBottom() {
		/*
			Function that rotates the bottom clockwise
		*/
		char frontSideBottomRow[3], backSideBottomRow[3], LeftSideBottomRow[3], RightSideBottomRow[3];
		char BottomSide[3][3];
		for (int i = 0; i < 3; i++) {
			frontSideBottomRow[i] = front->getSide()[2][i];
			backSideBottomRow[i] = back->getSide()[2][i];
			LeftSideBottomRow[i] = left->getSide()[2][i];
			RightSideBottomRow[i] = right->getSide()[2][i];
		}
		front->updateRow(2, LeftSideBottomRow);
		back->updateRow(2, RightSideBottomRow);
		right->updateRow(2, frontSideBottomRow);
		left->updateRow(2, backSideBottomRow);

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				BottomSide[i][j] = bottom->getSide()[i][j];

		bottom->updateCol(2, BottomSide[0]);
		bottom->updateCol(1, BottomSide[1]);
		bottom->updateCol(0, BottomSide[2]);
	}
};

/*
	GameState class
*/
class GameState {
	/*
		Contains a RubixCube, parent GameState cube pointer and the string which contains the move through which it was formed
	*/
	RubixCube* cube;
	GameState* parent;
	string move;
public:
	GameState() {
		cube = nullptr;
		parent = nullptr;
		move = "";
	}
	GameState(const GameState& state) {
		cube = new RubixCube(*state.cube);
		parent = state.parent;
		move = state.move;
	}
	GameState& operator = (const GameState& state) {
		/*
			Assignment Operator Overloaded
		*/
		cube = new RubixCube(*state.cube);
		parent = state.parent;
		move = state.move;
		return *this;
	}
	void setCube(RubixCube* cube) {
		/*
			sets the cube
		*/
		this->cube = new RubixCube(*cube);
	}
	RubixCube* getCube() {
		/*
			returns the cube
		*/
		return cube;
	}
	void setMove(string m) {
		/*
			sets the move
		*/
		move = m;
	}
	string getMove() {
		/*
			returns the move
		*/
		return move;
	}
	void setParent(GameState* parent) {
		/*
			sets the parent pointer
		*/
		this->parent = parent;
	}
	GameState* getParent() {
		/*
			returns the parent pointer
		*/
		return parent;
	}
	bool hasWon() {
		/*
			check if the cube has reached goal state
		*/
		return cube->allSidesFixed();
	}
	void moveCube(int move) {
		/*
			rotates the cube according to the number passed
		*/
		this->move = move == 1 ? "move front" : move == 2 ? "move back" :
			move == 3 ? "move right" : move == 4 ? "move left" :
			move == 5 ? "move top" : "move bottom";

		move == 1 ? cube->moveFront() : move == 2 ? cube->moveBack() :
			move == 3 ? cube->moveRight() : move == 4 ? cube->moveLeft() :
			move == 5 ? cube->moveTop() : cube->moveBottom();
	}
	void printCube() {
		/*
			prints the cube
		*/
		cube->printRubixCube(move);
	}
};

/*
	Game class
*/
class Game {
	/*
		contains a GameState pointer
	*/
	GameState* initialState;
public:
	Game(RubixCube* cube) {
		initialState = new GameState();
		initialState->setCube(cube);
		initialState->setParent(nullptr);
	}
	bool checkVisited(GameState& currState, vector<GameState*> visited) {
		/*
			function to check if a node is already visited or not
		*/
		for (int i = 0; i < visited.size(); i++)
			if (*(visited[i]->getCube()) == *(currState.getCube()))
				return true;
		return false;
	}
	void solveCube() {
		/*
			function that solves the cube
		*/
		gotoxy(0, 0);
		if (initialState->hasWon()) {
			cout << "Already solved" << endl;
			gotoxy(0, 20);
			return;
		}
		cout << "SOLVING" << endl;
		bool found = false;
		GameState* result = new GameState();
		vector<GameState*> visited;
		queue<GameState> bfsQueue;
		//parent of initial state is nullptr
		initialState->setParent(nullptr);
		bfsQueue.push(*initialState);
		//while queue is not empty and goal state has not been reached
		while (!bfsQueue.empty() && !found) {
			//get the front item state from the queue
			GameState* currentNode = new GameState((bfsQueue.front()));
			bfsQueue.pop();
			for (int i = 0; i < 6; i++) {
				//create 6 new states and apply different move to each,
				//set the parent to the current state popped from queue and push it in the queue
				GameState branch;
				branch.setCube(currentNode->getCube());
				branch.moveCube(i);
				branch.setParent(currentNode);
				if (branch.hasWon()) {
					//if the goal state is reached, set the result pointer and exit the loops
					result->setCube(branch.getCube());
					result->setParent(branch.getParent());
					result->setMove(branch.getMove());
					found = true;
					break;
				}
				//check if already visited
				if (checkVisited(branch, visited))
					continue;
				//else push it in queue
				bfsQueue.push(branch);
			}
			//mark as visited
			visited.push_back(currentNode);
		}
		//use stack to get set of moves in correct order
		stack<GameState*> stk;
		GameState* path = result;
		while (path) {
			stk.push(path);
			path = path->getParent();
		}
		//print the move and cube from initial state to goal state
		queue<string>movesToApply;
		while (!stk.empty()) {
			GameState* move = stk.top();
			move->printCube();
			movesToApply.push(move->getMove());
			Sleep(1000);
			stk.pop();
		}
		//print the moves list at the end
		gotoxy(0, 20);
		while (!movesToApply.empty()) {
			string move = movesToApply.front();
			movesToApply.pop();
			cout << move << endl;
		}
	}
};


/*
	Helper function to set the cube
	Cube is set to Goal state
*/
void createCube(RubixSide*& front, RubixSide*& back,
	RubixSide*& right, RubixSide*& left,
	RubixSide*& top, RubixSide*& bottom) {
	char** side = new char* [3];
	for (int i = 0; i < 3; i++)
		side[i] = new char[3];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'b';
	front->setSide(side);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'g';
	back->setSide(side);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'r';
	right->setSide(side);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'o';
	left->setSide(side);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'w';
	top->setSide(side);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			side[i][j] = 'y';
	bottom->setSide(side);
}

int main() {
	RubixSide* front = new RubixSide();
	RubixSide* back = new RubixSide();
	RubixSide* right = new RubixSide();
	RubixSide* left = new RubixSide();
	RubixSide* top = new RubixSide();
	RubixSide* bottom = new RubixSide();
	createCube(front, back, right, left, top, bottom);
	RubixCube* cube = new RubixCube(front, back, right, left, top, bottom);

	cube->moveFront();
	cube->moveFront();
	cube->moveFront();

	cube->moveTop();
	cube->moveTop();
	cube->moveTop();

	cube->moveBack();
	cube->moveBack();
	cube->moveBack();

	cube->moveBottom();
	cube->moveBottom();
	cube->moveBottom();

	cube->moveLeft();
	cube->moveLeft();
	cube->moveLeft();

	cube->moveRight();
	cube->moveRight();
	cube->moveRight();



	cube->printRubixCube();

	Game* game = new Game(cube);
	game->solveCube();
	return 0;
}
