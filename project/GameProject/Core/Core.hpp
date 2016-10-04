#ifndef CORE_HPP
#define CORE_HPP

#include "RenderEngine/IRenderEngine.hpp"
#include "Input.hpp"
#include "CameraInput.hpp"

#include "Instance.hpp"
#include "../Game/Game.hpp"

#include "Settings\DisplaySettings.hpp"

#undef APIENTRY

#include "../Loader/LibraryLoader.hpp"

class Core
{
	
public:

	void init();
	void release();

	bool hadGraphicsReset() const;

	void setFPS(int fps);
	void update(float dt);

	void render();

	DisplaySettings* getDisplaySettings();

private:

	bool hadReset;

	int fps;

	Console* console;
	Game* game;

	IRenderEngine* renderEngine;
	Input* input;
	CameraInput camInput;

	IShaderObject* shader;

	int vpLoc;
	int mdlLoc;
	int texLoc;

	IMesh* planeMesh;
	IMesh* triangleMesh;
	ICamera* camera;
	IFrameBuffer* fbo;
	
	ITexture* texture;

	bool toggle = true;
	int c;

	void* map;
	size_t mapSize;

	IText* text;
	IFont* font;

	DisplaySettings disp;

	Lib renderEngineLib;

};

#endif