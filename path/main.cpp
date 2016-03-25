#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "Node.h"
#include"../_resources/irrlicht-1.8.3/include/vector3d.h"
#include <driverChoice.h>
#include "ShaderCallback.h"
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>
#include "MyEventReceiver.h"

#define NODE_SIZE 0.3
#define EDGES 3
#define PATH_FOUND 0
#define NO_PATH 1
#define TB_X 0
#define TB_Y 1
#define TB_Z 2
int TB = NULL;
float func = (1 + sqrt(5)) / 2;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

// functions
void createNodes();
void createEdges();
void printNodes();
void run();
vector<Node*> shortestPath(Node* origin, Node* target);
void printPath(Node* origin, Node* node_ptr, vector<Node*>);
int aStar(std::map<Node*, Node*>* open_list, std::map<Node*, Node*>* closed_list, Node* current, Node* target);
Node* nextNode(std::map<Node*, Node*>* open_list, Node* target);
//void determineTieBreak(Node* start, Node* finish);
void analyseNode(Node* to_node, Node* current, int current_edge, Node* target);

// vars
std::vector<Node*> nodes;
std::vector<Node*> route;
Node* start;
Node* finish;

// This is the one method that we have to implement

int main() {
	
	run();

	UseHighLevelShaders = true;
	UseCgShaders = true;
	video::E_DRIVER_TYPE driverType = EDT_OPENGL;
	MyEventReceiver receiver;
	// create device
	device = createDevice(driverType, core::dimension2d<u32>(1024, 768), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* gui = device->getGUIEnvironment();
	

	// Make sure we don't try Cg without support for it
	if (UseCgShaders && !driver->queryFeature(video::EVDF_CG)) {
		printf("Warning: No Cg support, disabling.\n");
		UseCgShaders = false;
	}

	io::path vsFileName; // filename for the vertex shader
	io::path psFileName; // filename for the pixel shader

	//load shaders
	psFileName = "../_resources/irrlicht-1.8.3/media/opengl.frag";
	vsFileName = "../_resources/irrlicht-1.8.3/media/opengl.vert";

	

	if (!driver->queryFeature(video::EVDF_PIXEL_SHADER_1_1) &&
		!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Pixel shaders disabled "\
			"because of missing driver/hardware support.");
		psFileName = "";
	}

	if (!driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) &&
		!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Vertex shaders disabled "\
			"because of missing driver/hardware support.");
		vsFileName = "";
	}

	// create materials

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 newMaterialType1 = 0;
	s32 newMaterialType2 = 0;

	if (gpu)
	{
		ShaderCallback* mc = new ShaderCallback();

		// create the shaders depending on if the user wanted high level
		// or low level shaders:

		if (UseHighLevelShaders)
		{
			// Choose the desired shader type. Default is the native
			// shader type for the driver, for Cg pass the special
			// enum value EGSL_CG
			const video::E_GPU_SHADING_LANGUAGE shadingLanguage =
				UseCgShaders ? video::EGSL_CG : video::EGSL_DEFAULT;

			// create material from high level shaders (hlsl, glsl or cg)

			newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(
				vsFileName, "vertexMain", video::EVST_VS_1_1,
				psFileName, "pixelMain", video::EPST_PS_1_1,
				mc, video::EMT_SOLID, 0, shadingLanguage);

			newMaterialType2 = gpu->addHighLevelShaderMaterialFromFiles(
				vsFileName, "vertexMain", video::EVST_VS_1_1,
				psFileName, "pixelMain", video::EPST_PS_1_1,
				mc, video::EMT_TRANSPARENT_ADD_COLOR, 0, shadingLanguage);
		}
		else
		{
			// create material from low level shaders (asm or arb_asm)

			newMaterialType1 = gpu->addShaderMaterialFromFiles(vsFileName,
				psFileName, mc, video::EMT_SOLID);

			newMaterialType2 = gpu->addShaderMaterialFromFiles(vsFileName,
				psFileName, mc, video::EMT_TRANSPARENT_ADD_COLOR);
		}

		mc->drop();
	}

	// **** CREATE NODES
	for (int i = 0; i < nodes.size(); i++)
	{
		scene::ISceneNode* node = smgr->addSphereSceneNode(NODE_SIZE, 32, 0, i);
		node->setPosition(nodes[i]->pos);
		//node->setPosition(core::vector3df(0, -10, 50));
		node->setMaterialTexture(0, driver->getTexture("../_resources/irrlicht-1.8.3/media/wall.bmp"));
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
		node->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType1);
		
		smgr->addTextSceneNode(gui->getBuiltInFont(), std::to_wstring(nodes[i]->name).c_str(),
			video::SColor(255, 255, 255, 255), node, vector3df(0.0f, 0.0f, 0.4f));

		// ROTATION
		//scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.0, 0.0, 0.0));
		//node->addAnimator(anim);
		//anim->drop();

	}


	// *****

	// add a nice skybox

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	
	smgr->addSkyBoxSceneNode(
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_up.jpg"),
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_dn.jpg"),
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_lf.jpg"),
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_rt.jpg"),
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_ft.jpg"),
		driver->getTexture("../_resources/irrlicht-1.8.3/media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// add a camera and disable the mouse cursor

	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	cam->setPosition(core::vector3df(-0, 5, 10));
	cam->setTarget(core::vector3df(0, 0, 0));
	device->getCursorControl()->setVisible(false);

	video::S3DVertex Vertices[4];
	video::SMaterial Material;

	//driver->setMaterial(Material);

	int lastFPS = -1;

	const f32 MOVEMENT_SPEED = 5.f;

	u32 then = device->getTimer()->getTime();

	
	while (device->run())
		if (device->isWindowActive())
		{

			// Work out a frame delta time.
			const u32 now = device->getTimer()->getTime();
			const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
			then = now;

			vector3df camPosition = cam->getPosition();

			if (receiver.IsKeyDown(irr::KEY_KEY_W))
				cam->setPosition(vector3df(camPosition.X, camPosition.Y, camPosition.Z -= MOVEMENT_SPEED * frameDeltaTime));
			else if (receiver.IsKeyDown(irr::KEY_KEY_S))
				cam->setPosition(vector3df(camPosition.X , camPosition.Y, camPosition.Z += MOVEMENT_SPEED * frameDeltaTime));

			if (receiver.IsKeyDown(irr::KEY_KEY_A))
				cam->setPosition(vector3df(camPosition.X += MOVEMENT_SPEED * frameDeltaTime, camPosition.Y, camPosition.Z));
			else if (receiver.IsKeyDown(irr::KEY_KEY_D))
				cam->setPosition(vector3df(camPosition.X -= MOVEMENT_SPEED * frameDeltaTime, camPosition.Y, camPosition.Z));
			
			/////////BEGIN
			driver->beginScene(true, true, video::SColor(0, 100, 100, 100));
			smgr->drawAll();
			
			driver->setTransform(ETS_WORLD, matrix4());
			
			// ***** EDGES
			for (int i = 0; i < nodes.size(); i++) {
				for (int j = 0; j < nodes[i]->edges.size(); j++) {
					driver->draw3DLine(nodes[i]->pos, nodes[i]->edges[j].to_node->pos, SColor(255, 125, 20, 125));
				}
			}
			
			for (int i = 0; i < route.size(); i++) {
				smgr->getSceneNodeFromId(route[i]->name)->setMaterialTexture(0, driver->getTexture("../_resources/irrlicht-1.8.3/media/portal2.bmp"));
				smgr->getSceneNodeFromId(route[i]->name)->setMaterialFlag(video::EMF_BLEND_OPERATION, false);
				smgr->getSceneNodeFromId(route[i]->name)->setMaterialFlag(video::EMF_LIGHTING, false);

			}
			driver->endScene();
			/////////ENND

			int fps = driver->getFPS();
			if (lastFPS != fps)
			{
				core::stringw str = L"Irrlicht Engine - Vertex and pixel shader example [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
	device->drop();
	return 0;
}


void run() {

	createNodes();
	createEdges();
	printNodes();

	start = nodes[0];
	finish = nodes[1];
	route = shortestPath(start, finish);
	
	if ( route.size() > 0) {
		cout << "PATH FOUND" << endl;;
		printPath(start, finish, route);
	}
	else {
		cout << "NO PATH" << endl;
	}
	
}

void createNodes() {
	
	nodes.push_back(new Node(vector3df(0, 1, 3 * func)));
	nodes.push_back(new Node(vector3df(0, 1, -3 * func)));
	nodes.push_back(new Node(vector3df(0, -1, 3 * func)));
	nodes.push_back(new Node(vector3df(0, -1, -3 * func)));
	nodes.push_back(new Node(vector3df(1, 3 * func, 0)));
	nodes.push_back(new Node(vector3df(1, -3 * func, 0)));
	nodes.push_back(new Node(vector3df(-1, 3 * func, 0)));
	nodes.push_back(new Node(vector3df(-1, -3 * func, 0)));
	nodes.push_back(new Node(vector3df(3 * func, 0, 1)));
	nodes.push_back(new Node(vector3df(3 * func, 0, -1)));
	nodes.push_back(new Node(vector3df(-3 * func, 0, 1)));
	nodes.push_back(new Node(vector3df(-3 * func, 0, -1)));
	nodes.push_back(new Node(vector3df(2, (1 + 2 * func), func)));
	nodes.push_back(new Node(vector3df(2, (1 + 2 * func), -func)));
	nodes.push_back(new Node(vector3df(2, -(1 + 2 * func), func)));
	nodes.push_back(new Node(vector3df(2, -(1 + 2 * func), -func)));
	nodes.push_back(new Node(vector3df(-2, (1 + 2 * func), func)));
	nodes.push_back(new Node(vector3df(-2, (1 + 2 * func), -func)));
	nodes.push_back(new Node(vector3df(-2, -(1 + 2 * func), func)));
	nodes.push_back(new Node(vector3df(-2, -(1 + 2 * func), -func)));
	nodes.push_back(new Node(vector3df((1 + 2 * func), func, 2)));
	nodes.push_back(new Node(vector3df((1 + 2 * func), func, -2)));
	nodes.push_back(new Node(vector3df((1 + 2 * func), -func, 2)));
	nodes.push_back(new Node(vector3df((1 + 2 * func), -func, -2)));
	nodes.push_back(new Node(vector3df(-(1 + 2 * func), func, 2)));
	nodes.push_back(new Node(vector3df(-(1 + 2 * func), func, -2)));
	nodes.push_back(new Node(vector3df(-(1 + 2 * func), -func, 2)));
	nodes.push_back(new Node(vector3df(-(1 + 2 * func), -func, -2)));
	nodes.push_back(new Node(vector3df(func, 2, (1 + 2 * func))));
	nodes.push_back(new Node(vector3df(func, 2, -(1 + 2 * func))));
	nodes.push_back(new Node(vector3df(func, -2, (1 + 2 * func))));
	nodes.push_back(new Node(vector3df(func, -2, -(1 + 2 * func))));
	nodes.push_back(new Node(vector3df(-func, 2, (1 + 2 * func))));
	nodes.push_back(new Node(vector3df(-func, 2, -(1 + 2 * func))));
	nodes.push_back(new Node(vector3df(-func, -2, (1 + 2 * func))));
	nodes.push_back(new Node(vector3df(-func, -2, -(1 + 2 * func))));
	nodes.push_back(new Node(vector3df(1, (2 + func), 2 * func)));
	nodes.push_back(new Node(vector3df(1, (2 + func), -2 * func)));
	nodes.push_back(new Node(vector3df(1, -(2 + func), 2 * func)));
	nodes.push_back(new Node(vector3df(1, -(2 + func), -2 * func)));
	nodes.push_back(new Node(vector3df(-1, (2 + func), 2 * func)));
	nodes.push_back(new Node(vector3df(-1, (2 + func), -2 * func)));
	nodes.push_back(new Node(vector3df(-1, -(2 + func), 2 * func)));
	nodes.push_back(new Node(vector3df(-1, -(2 + func), -2 * func)));
	nodes.push_back(new Node(vector3df((2 + func), 2 * func, 1)));
	nodes.push_back(new Node(vector3df((2 + func), 2 * func, -1)));
	nodes.push_back(new Node(vector3df((2 + func), -2 * func, 1)));
	nodes.push_back(new Node(vector3df((2 + func), -2 * func, -1)));
	nodes.push_back(new Node(vector3df(-(2 + func), 2 * func, 1)));
	nodes.push_back(new Node(vector3df(-(2 + func), 2 * func, -1)));
	nodes.push_back(new Node(vector3df(-(2 + func), -2 * func, 1)));
	nodes.push_back(new Node(vector3df(-(2 + func), -2 * func, -1)));
	nodes.push_back(new Node(vector3df(2 * func, 1, (2 + func))));
	nodes.push_back(new Node(vector3df(2 * func, 1, -(2 + func))));
	nodes.push_back(new Node(vector3df(2 * func, -1, (2 + func))));
	nodes.push_back(new Node(vector3df(2 * func, -1, -(2 + func))));
	nodes.push_back(new Node(vector3df(-2 * func, 1, (2 + func))));
	nodes.push_back(new Node(vector3df(-2 * func, 1, -(2 + func))));
	nodes.push_back(new Node(vector3df(-2 * func, -1, (2 + func))));
	nodes.push_back(new Node(vector3df(-2 * func, -1, -(2 + func))));

	for (int i = 0; i < nodes.size(); i++){
		nodes[i]->name = i;
	}

}

void createEdges() {
	// for all nodes
	for (int node_from = 0; node_from < nodes.size(); node_from++) {
		// for all nodes
		for (int node_to = 0; node_to < nodes.size(); node_to++) {
			// if the nodes are not the same object
			if (node_from != node_to) {
				// calculate the distance between the two nodes
				float cost = nodes[node_to]->pos.getDistanceFrom(nodes[node_from]->pos);
				// if the nodes doesnt have 3 edges yet - add the new edge
				if (nodes[node_from]->edges.size() < 3) {
					nodes[node_from]->edges.push_back(Edge{ nodes[node_to], cost });
				}
				// else - continue to work out where to place the new edge
				else {
					// default the max index and cost to the first node in the edge array
					int max_index = 0;
					float max_cost = nodes[node_from]->edges[0].cost;
					// for all edges of the node excluding the first  (this has already been considered as the default)
					for (int edge = 1; edge < nodes[node_from]->edges.size(); edge++) {
						// if the edge cost is greater than the maximum; record the index and max cost
						if (nodes[node_from]->edges[edge].cost > max_cost) {
							max_index = edge;
							max_cost = nodes[node_from]->edges[edge].cost;
						}
					}
					// if the cost of the new edge is LESS than the cost of the most expensive edge; overwrite the most expensive edge in place of the current one.
					if (cost < max_cost) {
						nodes[node_from]->edges[max_index].to_node = nodes[node_to];
						nodes[node_from]->edges[max_index].cost = cost;
					}
				}

			}
		}
	}
}

void printNodes() {

	for (int i = 0; i < nodes.size(); i++) {
		cout << "Node " << nodes[i]->name << " {" << nodes[i] << "} {" << *nodes[i] << "}" << endl;
	}
}


vector<Node*> shortestPath (Node* origin, Node* target) {
	std::map<Node*, Node*> open_list;
	std::map<Node*, Node*> closed_list;
	vector<Node*> final_path;
	vector<Node*> reverse_path;

	Node* current = origin;
	current->g = 0;
	open_list.insert(std::pair<Node*, Node*>(current, current));
	
	//determineTieBreak(origin, target);

	int route = aStar(&open_list, &closed_list, current, target);

	if (route == PATH_FOUND) {
		Node* node_ptr = finish;
		while (node_ptr != start) {
			final_path.push_back(node_ptr);
			node_ptr = node_ptr->parent;
		}
		final_path.push_back(start);

		return final_path;
	}
	else
		exit(1);
}
/*
void determineTieBreak(Node* start, Node* finish) {
	float x = start->pos.X - finish->pos.X;
	float y = start->pos.Y - finish->pos.Y;
	float z = start->pos.Z - finish->pos.Z;

	int tieBreak = 0;

	if (abs(x) > tieBreak)
		TB = TB_X;

	if (abs(y) > tieBreak)
		TB = TB_Y;

	if (abs(z) > tieBreak)
		TB = TB_Z;
}
*/
void analyseNode(Node* to_node, Node* current, int current_edge, Node* target) {
	to_node->h = to_node->pos.getDistanceFrom(target->pos);
	to_node->g = current->g + current->edges[current_edge].cost;
	to_node->f = to_node->h + to_node->g;
}

int aStar(std::map<Node*, Node*>* open_list, std::map<Node*, Node*>* closed_list, Node* current, Node* target) {

	for (int i = 0; i < EDGES; i++)
	{
		Node* to_node = current->edges[i].to_node;

		// add all edges into open list
		if (closed_list->count(to_node) == 0 && to_node->passable == true) {
			if (open_list->count(to_node) == 0) {
				open_list->insert(std::pair<Node*, Node*>(to_node, current));
				to_node->parent = current;
				analyseNode(to_node, current, i, target);
			}
			else {
				float temp_g_score = current->g + current->edges[i].cost;
				if (temp_g_score < to_node->g) {
					analyseNode(to_node, (*open_list)[to_node], i, target);
				}
			}
		}
		// else ignore, this node already been evaluated
	}
	
	std::map<Node*, Node*>::iterator it;
	// find the current node being evaluated
	it = (*open_list).find(current);
	// move the current node to the closed list so that it isnt evaluated further
	(*closed_list).insert(std::pair<Node*, Node*>(it->first, it->second));
	// if the current node is equal to the target node -- path found
	if (it->first == target) return PATH_FOUND;
	// remove current node form open list so that it isnt evaluated again
	(*open_list).erase(it);
	// if there are now no nodes left in the open list, it means that there is no path from current node to target node: exit algorithm
	if ((*open_list).size() == 0) return NO_PATH;
	// call the function again with the node that has the lowest f score in the open list
	
	aStar(open_list, closed_list, nextNode(open_list, target), target);
}

Node* nextNode(std::map<Node*, Node*>* open_list, Node* target) {
	assert((*open_list).size() > 0);
	// TODO check for empty open list 
	Node* lowest_score = (*open_list).begin()->first;
	for (std::map<Node*, Node*>::iterator it = (*open_list).begin(); it != (*open_list).end(); ++it) {
		if (it->first->f < lowest_score->f)
			lowest_score = it->first;
	}
	return lowest_score;
}

void printPath(Node* origin, Node* node_ptr, vector<Node*> path) {
	
	for (int i = 0; i < path.size(); i++){
		cout << i << endl;
	}
}