/*******************************************************************/
/**************    AStar_Algo.cpp      *****************************/
/************** Author: Stephen Liu ********************************/
/************** Email: stephen.gta@gmail.com ***********************/
/************** Phone: 6475270659 **********************************/
/************** Created at: May,2020 *******************************/

#include "AStar_Algo.h"
#include <mmsystem.h>//playsound
//#include "UpdatedMapEditorView.h"

bool operator<(const node & a, const node & b)
{
	return a.getPriority() > b.getPriority();
}

AStar_Algo::AStar_Algo(int n, int m, int gridWidth, int max): m_sx(0),m_sy(0),m_ex(n-1),m_ey(m-1),
m_dm(m), m_dn(n), m_GridWidth(gridWidth), m_maxSearchDistance(max)
{
	srand(time(NULL));
	map = new int[n*m];
	originalMap = new int[n*m]; 
	// create empty map
	for (int y = 0; y<m; y++)
	{
		for (int x = 0; x < n; x++)
		{
			map[x*m + y] = 0;
			originalMap[x*m + y] = 0;
		}

	}

	dir_map = new int[n*m];
	open_nodes_map = new int[n*m];
	closed_nodes_map = new int[n*m];
}


AStar_Algo::~AStar_Algo()
{
	delete[] map;
	delete[] dir_map;
	delete[] open_nodes_map;
	delete[] closed_nodes_map;
}

void AStar_Algo::clearMap()
{
	for (int y = 0; y<m_dm; y++)
	{
		for (int x = 0; x<m_dn; x++) map[x*m_dm+y] = 0;
	}
}

void AStar_Algo::setObstacle(int x1, int y1, int potential)
{
	if (x1 >= m_dn || y1 >= m_dm ) return;
	map[x1*m_dm+y1] = 1;
	/*
	// fillout the map matrix with a '+' pattern
	for (int x = n / 8; x<n * 7 / 8; x++)
	{
		map[x][m / 2] = 1;
	}
	for (int y = m / 8; y<m * 7 / 8; y++)
	{
		map[n / 2][y] = 1;
	}
	*/

}

void AStar_Algo::clearObstacle(int x1, int y1)
{
	if (x1 >= m_dn || y1 >= m_dm) return;
	map[x1*m_dm+y1] = 0;
}

bool AStar_Algo::isObstacle(int x1, int y1)
{
	if (x1 >= m_dn || y1 >= m_dm || x1 <0 || y1<0) return true;
	return (map[x1*m_dm + y1]==1);
}

void AStar_Algo::setOriginalMapObstacle(int x1, int y1)
{
	if (x1 >= m_dn || y1 >= m_dm) return;
	originalMap[x1*m_dm + y1] = 1;
}

bool AStar_Algo::isOriginalMapObstacle(int x1, int y1)
{
	if (x1 >= m_dn || y1 >= m_dm) return true;
	return (originalMap[x1*m_dm + y1] == 1);
}
string AStar_Algo::pathFind(const int & xStart, const int & yStart,
	const int & xFinish, const int & yFinish)
{
	priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
	int pqi; // pq index
	node* n0;
	node* m0;
	int i, j, x, y, xdx, ydy;
	char c;
	pqi = 0;
	const DWORD PATH_FIND_DURATION = 2000;
	DWORD start = GetTickCount();
	// reset the node maps
	for (y = 0; y<m_dm; y++)
	{
		for (x = 0; x<m_dn; x++)
		{
			closed_nodes_map[x*m_dm+y] = 0;
			open_nodes_map[x*m_dm+y] = 0;
		}
	}

	// create the start node and push into list of open nodes
	n0 = new node(xStart, yStart, 0, 0);
	n0->updatePriority(xFinish, yFinish);
	pq[pqi].push(*n0);
	//open_nodes_map[x*m_dm+y] = n0->getPriority(); // mark it on the open nodes map

	open_nodes_map[xStart*m_dm + yStart] = n0->getPriority(); // stephen test on Jul 4th 2016
	delete n0; // stephen test on Jul 4th 2016
	// A* search
	while (!pq[pqi].empty())
	{
		if (GetTickCount() - start > PATH_FIND_DURATION) // 2000ms should finish path find loop
		{
			PlaySound(TEXT("algorithmlongtime.wav"), NULL, SND_FILENAME);
			break;
		}

		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
			pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

		x = n0->getxPos(); y = n0->getyPos();
		int distance  = sqrt((x - xStart)*(x - xStart) + (y - yStart)*(y - yStart));
		if (distance > m_maxSearchDistance)
		{
			PlaySound(TEXT("searchdistancefar.wav"), NULL, SND_FILENAME);
			return "";
		}

		pq[pqi].pop(); // remove the node from the open list
		open_nodes_map[x*m_dm+y] = 0;
		// mark it on the closed nodes map
		closed_nodes_map[x*m_dm+y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(xFinish, yFinish) == 0)
		if (x == xFinish && y == yFinish)
		{
			// generate the path from finish to start
			// by following the directions
			string path = "";
			while (!(x == xStart && y == yStart))
			{
				j = dir_map[x*m_dm+y];// [x][y];
				c = '0' + (j + dir / 2) % dir;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			// garbage collection
			delete n0;
			// empty the leftover nodes
			while (!pq[pqi].empty()) pq[pqi].pop();
			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i<dir; i++)
		{
			xdx = x + dx[i]; ydy = y + dy[i];

			if (!(xdx<0 || xdx>m_dn - 1 || ydy<0 || ydy>m_dm - 1 || map[xdx*m_dm+ydy] == 1
				|| closed_nodes_map[xdx*m_dm+ydy] == 1))
			{
				// generate a child node
				m0 = new node(xdx, ydy, n0->getLevel(),
					n0->getPriority());
				m0->nextLevel(i);
				m0->updatePriority(xFinish, yFinish); 

				// if it is not in the open list then add into that
				if (open_nodes_map[xdx*m_dm+ydy] == 0)
				{
					open_nodes_map[xdx*m_dm+ydy] = m0->getPriority();
					pq[pqi].push(*m0);
					delete m0; // stephen memory leak test on Jul 4th, 2016
					// mark its parent node direction
					dir_map[xdx*m_dm +ydy] = (i + dir / 2) % dir;
				}
				else if (open_nodes_map[xdx*m_dm +ydy]>m0->getPriority())
				{
					// update the priority info
					open_nodes_map[xdx*m_dm +ydy] = m0->getPriority();
					// update the parent direction info
					dir_map[xdx*m_dm +ydy] = (i + dir / 2) % dir;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().getxPos() == xdx &&
						pq[pqi].top().getyPos() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pq[pqi].pop(); // remove the wanted node

								   // empty the larger size pq to the smaller one
					if (pq[pqi].size()>pq[1 - pqi].size()) pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
					delete m0; //stephen test on Jul 4th, 2016
				}
				else delete m0; // garbage collection
			}
		}
		delete n0; // garbage collection
	}
	return ""; // no route found
}

void AStar_Algo::setStartAndTarget(int xA, int yA, int xB, int yB)
{
	m_sx = xA;
	m_sy = yA;
	m_ex = xB;
	m_ey = yB;
}

int AStar_Algo::run()
{
	// randomly select start and finish locations
	//int xA = 29, yA = 29, xB = 31, yB = 31;

	string route = pathFind(m_sx, m_sy, m_ex, m_ey);

	// follow the route on the map and display it 
	if (route.length()>0)
	{
		int j; char c;
		int x = m_sx;
		int y = m_sy;
		map[x*m_dm +y] = 2;
		m_paths.push_back(coord(x, y));
		for (int i = 0; i<route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x*m_dm +y] = 3;
			 
			m_paths.push_back(coord(x,y));
		}
		map[x*m_dm +y] = 4;
		m_paths.push_back(coord(x, y));
	}
	return(route.length());
}
#if 0
int AStar_Algo::run()
{
	// randomly select start and finish locations
	int xA, yA, xB, yB;
	switch (rand() % 8)
	{
	case 0: xA = 0; yA = 0; xB = n - 1; yB = m - 1; break;
	case 1: xA = 0; yA = m - 1; xB = n - 1; yB = 0; break;
	case 2: xA = n / 2 - 1; yA = m / 2 - 1; xB = n / 2 + 1; yB = m / 2 + 1; break;
	case 3: xA = n / 2 - 1; yA = m / 2 + 1; xB = n / 2 + 1; yB = m / 2 - 1; break;
	case 4: xA = n / 2 - 1; yA = 0; xB = n / 2 + 1; yB = m - 1; break;
	case 5: xA = n / 2 + 1; yA = m - 1; xB = n / 2 - 1; yB = 0; break;
	case 6: xA = 0; yA = m / 2 - 1; xB = n - 1; yB = m / 2 + 1; break;
	case 7: xA = n - 1; yA = m / 2 + 1; xB = 0; yB = m / 2 - 1; break;
	}

	cout << "Map Size (X,Y): " << n << "," << m << endl;
	cout << "Start: " << xA << "," << yA << endl;
	cout << "Finish: " << xB << "," << yB << endl;
	// get the route
	clock_t start = clock();
	string route = pathFind(xA, yA, xB, yB);
	if (route == "") cout << "An empty route generated!" << endl;
	clock_t end = clock();
	double time_elapsed = double(end - start);
	cout << "Time to calculate the route (ms): " << time_elapsed << endl;
	cout << "Route:" << endl;
	cout << route << endl << endl;

	// follow the route on the map and display it 
	if (route.length()>0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		map[x][y] = 2;
		for (int i = 0; i<route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
		}
		map[x][y] = 4;

		// display the map with the route
		for (int y = 0; y<m; y++)
		{
			for (int x = 0; x<n; x++)
				if (map[x][y] == 0)
					cout << ".";
				else if (map[x][y] == 1)
					cout << "O"; //obstacle
				else if (map[x][y] == 2)
					cout << "S"; //start
				else if (map[x][y] == 3)
					cout << "R"; //route
				else if (map[x][y] == 4)
					cout << "F"; //finish
			cout << endl;
		}
	}
	getchar(); // wait for a (Enter) keypress  
	return(0);
}
#endif