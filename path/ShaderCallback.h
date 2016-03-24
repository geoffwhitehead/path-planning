#pragma once
#include <irrlicht.h>
#include <iostream>
#include "driverChoice.h"
#include "IShaderConstantSetCallBack.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

IrrlichtDevice* device = 0;
bool UseHighLevelShaders = false;
bool UseCgShaders = false;

class ShaderCallback :
	public video::IShaderConstantSetCallBack
{
public:


	virtual void OnSetConstants(video::IMaterialRendererServices* services,s32 userData){
		video::IVideoDriver* driver = services->getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
		else
			services->setVertexShaderConstant(invWorld.pointer(), 0, 4);

		// set clip matrix

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else
			services->setVertexShaderConstant(worldViewProj.pointer(), 4, 4);

		// set camera position

		core::vector3df pos = device->getSceneManager()->
			getActiveCamera()->getAbsolutePosition();

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);
		else
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&pos), 8, 1);

		video::SColorf col(0.0f, 0.4f, 0.8f, 0.5f);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mLightColor",
				reinterpret_cast<f32*>(&col), 4);
		else
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&col), 9, 1);

		// set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();

		if (UseHighLevelShaders)
		{
			services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

			// set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
			s32 TextureLayerID = 0;
			if (UseHighLevelShaders)
				services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);
		}
		else
			services->setVertexShaderConstant(world.pointer(), 10, 4);
	}
};

