/*******************************************************************/
/**************    AStar_Algo.h        *****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/
#pragma once
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <vector>
using namespace std;
//const int g_GridWidth = 16;
//const int n = 800 / g_GridWidth;  //60; // horizontal size of the map
//const int m = 1000 / g_GridWidth; // 60; // vertical size size of the map
const int dir = 8; // number of possible directions to go at any position

				   // Determine priority (in the priority queue)
class node
{
	// current position
	int xPos;
	int yPos;
	// total distance already travelled to reach the node
	int level;
	// priority=level+remaining distance estimate
	int priority;  // smaller: higher priority

public:
	node(int xp, int yp, int d, int p)
	{
		xPos = xp; yPos = yp; level = d; priority = p;
	}

	int getxPos() const { return xPos; }
	int getyPos() const { return yPos; }
	int getLevel() const { return level; }
	int getPriority() const { return priority; }

	void updatePriority(const int & xDest, const int & yDest)
	{
		priority = level + estimate(xDest, yDest) * 10; //A*
	}

	// give better priority to going strait instead of diagonally
	void nextLevel(const int & i) // i: direction
	{
		level += (dir == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	// Estimation function for the remaining distance to the goal.
	const int & estimate(const int & xDest, const int & yDest) const
	{
		static int xd, yd, d;
		xd = xDest - xPos;
		yd = yDest - yPos;

		// Euclidian Distance
		d = static_cast<int>(sqrt(xd*xd + yd*yd));

		// Manhattan distance
		//d=abs(xd)+abs(yd);

		// Chebyshev distance
		//d=max(abs(xd), abs(yd));

		return(d);
	}
};

class AStar_Algo
{
public:
	struct coord
	{
		coord() :x(-1), y(-1) {};
		coord(int i, int j) :x(i), y(j) {};
		int x;
		int y;
		bool const operator==(const coord &o) {
			return x == o.x && y == o.y;
		}

		bool  operator<(const coord &o) const {
			return x < o.x || (x == o.x && y < o.y);
		}

	};
	AStar_Algo(int n, int m, int gridWidth, int max);
	~AStar_Algo();
	void setStartAndTarget(int xA, int yA, int xB, int yB);
	void setObstacle(int x1, int y1, int potential=1);
	void setOriginalMapObstacle(int x1, int y1);
	bool isOriginalMapObstacle(int x1, int y1);
	void clearObstacle(int x1, int y1);
	bool isObstacle(int x1, int y1);
	string pathFind(const int & xStart, const int & yStart,
		const int & xFinish, const int & yFinish);
	int run();
	void clearMap();
	int getM() { return m_dm; }
	int getN() { return m_dn; }
	int getGridWidth() { return m_GridWidth; }
	void setMaxSearchDistance(int max) { m_maxSearchDistance = max; }
	std::vector<coord> m_paths;
	std::vector<coord> m_mergedPaths;
	std::vector<coord> m_amendedPaths;
	int* map; // [n][m];
	int* originalMap;
	typedef std::vector<coord>::iterator pathIter;
private:
	// Determine priority (in the priority queue)

	int* closed_nodes_map;// [n][m]; // map of closed (tried-out) nodes
	int* open_nodes_map;// [n][m]; // map of open (not-yet-tried) nodes
	int* dir_map; // map of directions

	int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int m_sx;
	int m_sy;
	int m_ex;
	int m_ey;
	int m_GridWidth;
	int m_dm;
	int m_dn;
	int m_maxSearchDistance;
};





