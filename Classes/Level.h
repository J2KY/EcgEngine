#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class Level : public Node
{
public:

	TMXTiledMap* _map;

	void loadMap(const char* name);
	TMXTiledMap* getMap();

	Point positionForTileCoordinate(Size s, Point point);

	Level(void);
	virtual ~Level(void);
};