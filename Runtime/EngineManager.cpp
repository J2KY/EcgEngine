#include "EngineManager.h"

//Static variable init
std::unique_ptr<EngineManager> EngineManager::_instance = nullptr;

EngineManager* EngineManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = make_unique<EngineManager>();
	}

	return _instance.get();
}

void EngineManager::SetDefaultLayer(shared_ptr<DefaultLayer> defaultLayer)
{
	_defaultLayer = defaultLayer;
}

DefaultLayer* EngineManager::GetDefaultLayer()
{
	return _defaultLayer.get();
}