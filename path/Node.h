#pragma once

#include <vector>
#include "../_resources/irrlicht-1.8.3/include/vector3d.h"

using namespace irr;
using namespace core;

struct Edge {
	Node* to_node;
	float cost;
};

class Node {

public:
	Node(vector3df);

private:
	vector3df pos;
	int h;
	int g;
	int f;
	int edge_cost;
	std::vector<Edge> edges;

};