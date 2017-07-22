#ifndef IASSETMANAGER_HPP
#define IASSETMANAGER_HPP

#include "FileStructs.hpp"

static const char* RENDERLAYER_TAG = "REND";
static const char* SCENE_TAG = "SCNE";

class IAssetManager {

public:

	virtual void createNewEntry(uint32_t formID, const char* tag) = 0;
	virtual Entry* getEntry(uint32_t formID) = 0;
	virtual void updateEntry(Entry* entry) = 0;

	virtual void setStartupSceneRef(uint32_t ref) = 0;
	virtual uint32_t getStartupSceneRef() = 0;

};

#endif