#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "Node.h"
#include"../_resources/irrlicht-1.8.3/include/vector3d.h"

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

void createNodes();

std::vector<Node> nodes;




float func() {
	return (1 + sqrt(5)) / 2;
}

int main() {

	createNodes();

	IrrlichtDevice *device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(640, 480), 16, false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		core::rect<s32>(10, 10, 260, 22), true);

	core::
	/*
	scene::IAnimatedMesh* mesh = smgr->getMesh("../_resources/irrlicht-1.8.3/media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	if (node) {
		node->setMaterialFlag(video::EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0, driver->getTexture("../_resources/irrlicht-1.8.3/media/sydney.bmp"));
	}
	*/


	smgr->addCameraSceneNode(0, core::vector3df(0, 30, -40), core::vector3df(0, 5, 0));

	while (device->run()) {
		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

	device->drop();

	return 0;
	


	Node start = nodes[1];
	Node end = nodes[10];




}


void createNodes() {
	
	nodes.push_back(Node(vector3df(0, 1, 3 * func())));
	nodes.push_back(Node(vector3df(0, 1, -3 * func())));
	nodes.push_back(Node(vector3df(0, -1, 3 * func())));
	nodes.push_back(Node(vector3df(0, -1, -3 * func())));
	nodes.push_back(Node(vector3df(1, 3 * func(), 0)));
	nodes.push_back(Node(vector3df(1, -3 * func(), 0)));
	nodes.push_back(Node(vector3df(-1, 3 * func(), 0)));
	nodes.push_back(Node(vector3df(-1, -3 * func(), 0)));
	nodes.push_back(Node(vector3df(3 * func(), 0, 1)));
	nodes.push_back(Node(vector3df(3 * func(), 0, -1)));
	nodes.push_back(Node(vector3df(-3 * func(), 0, 1)));
	nodes.push_back(Node(vector3df(-3 * func(), 0, -1)));
	nodes.push_back(Node(vector3df(2, (1 + 2 * func()), func())));
	nodes.push_back(Node(vector3df(2, (1 + 2 * func()), -func())));
	nodes.push_back(Node(vector3df(2, -(1 + 2 * func()), func())));
	nodes.push_back(Node(vector3df(2, -(1 + 2 * func()), -func())));
	nodes.push_back(Node(vector3df(-2, (1 + 2 * func()), func())));
	nodes.push_back(Node(vector3df(-2, (1 + 2 * func()), -func())));
	nodes.push_back(Node(vector3df(-2, -(1 + 2 * func()), func())));
	nodes.push_back(Node(vector3df(-2, -(1 + 2 * func()), -func())));
	nodes.push_back(Node(vector3df((1 + 2 * func()), func(), 2)));
	nodes.push_back(Node(vector3df((1 + 2 * func()), func(), -2)));
	nodes.push_back(Node(vector3df((1 + 2 * func()), -func(), 2)));
	nodes.push_back(Node(vector3df((1 + 2 * func()), -func(), -2)));
	nodes.push_back(Node(vector3df(-(1 + 2 * func()), func(), 2)));
	nodes.push_back(Node(vector3df(-(1 + 2 * func()), func(), -2)));
	nodes.push_back(Node(vector3df(-(1 + 2 * func()), -func(), 2)));
	nodes.push_back(Node(vector3df(-(1 + 2 * func()), -func(), -2)));
	nodes.push_back(Node(vector3df(func(), 2, (1 + 2 * func()))));
	nodes.push_back(Node(vector3df(func(), 2, -(1 + 2 * func()))));
	nodes.push_back(Node(vector3df(func(), -2, (1 + 2 * func()))));
	nodes.push_back(Node(vector3df(func(), -2, -(1 + 2 * func()))));
	nodes.push_back(Node(vector3df(-func(), 2, (1 + 2 * func()))));
	nodes.push_back(Node(vector3df(-func(), 2, -(1 + 2 * func()))));
	nodes.push_back(Node(vector3df(-func(), -2, (1 + 2 * func()))));
	nodes.push_back(Node(vector3df(-func(), -2, -(1 + 2 * func()))));
	nodes.push_back(Node(vector3df(1, (2 + func()), 2 * func())));
	nodes.push_back(Node(vector3df(1, (2 + func()), -2 * func())));
	nodes.push_back(Node(vector3df(1, -(2 + func()), 2 * func())));
	nodes.push_back(Node(vector3df(1, -(2 + func()), -2 * func())));
	nodes.push_back(Node(vector3df(-1, (2 + func()), 2 * func())));
	nodes.push_back(Node(vector3df(-1, (2 + func()), -2 * func())));
	nodes.push_back(Node(vector3df(-1, -(2 + func()), 2 * func())));
	nodes.push_back(Node(vector3df(-1, -(2 + func()), -2 * func())));
	nodes.push_back(Node(vector3df((2 + func()), 2 * func(), 1)));
	nodes.push_back(Node(vector3df((2 + func()), 2 * func(), -1)));
	nodes.push_back(Node(vector3df((2 + func()), -2 * func(), 1)));
	nodes.push_back(Node(vector3df((2 + func()), -2 * func(), -1)));
	nodes.push_back(Node(vector3df(-(2 + func()), 2 * func(), 1)));
	nodes.push_back(Node(vector3df(-(2 + func()), 2 * func(), -1)));
	nodes.push_back(Node(vector3df(-(2 + func()), -2 * func(), 1)));
	nodes.push_back(Node(vector3df(-(2 + func()), -2 * func(), -1)));
	nodes.push_back(Node(vector3df(2 * func(), 1, (2 + func()))));
	nodes.push_back(Node(vector3df(2 * func(), 1, -(2 + func()))));
	nodes.push_back(Node(vector3df(2 * func(), -1, (2 + func()))));
	nodes.push_back(Node(vector3df(2 * func(), -1, -(2 + func()))));
	nodes.push_back(Node(vector3df(-2 * func(), 1, (2 + func()))));
	nodes.push_back(Node(vector3df(-2 * func(), 1, -(2 + func()))));
	nodes.push_back(Node(vector3df(-2 * func(), -1, (2 + func()))));
	nodes.push_back(Node(vector3df(-2 * func(), -1, -(2 + func()))));

}