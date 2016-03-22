#pragma once

#include <iostream>
#include <vector>
#include "../_resources/irrlicht-1.8.3/include/vector3d.h"
#include <string>

using namespace irr;
using namespace core;
using namespace std;

struct Node;

struct Edge {
	Node* to_node;
	float cost;
};

class Node {

public:
	Node(vector3df);
	vector3df pos;
	int h;
	int g;
	int f;
	Node* parent;
	std::vector<Edge> edges;
	bool passable = true;

	friend ostream &operator<<(ostream &output,
		const Node N)
	{
		output
			<< "Node 1:"
			<< " Edge 1: " << N.edges[0].to_node << " Cost " << N.edges[0].cost
			<< " Edge 2: " << N.edges[1].to_node << " Cost " << N.edges[1].cost
			<< " Edge 3: " << N.edges[2].to_node << " Cost " << N.edges[2].cost;
		return output;
	}
};