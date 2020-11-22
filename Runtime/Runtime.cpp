#include <iostream>
#include <direct.h>

#include "AppDelegate.h"
#include "Runtime.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "Level.h"
#include "GameObject.h"
#include "DefaultLayer.h"

using namespace cocos2d;

//TODO : ���� parent�� �����ϴ� �뵵�θ� ����ϵ��� ����
void Runtime::initialize(int parent)
{
	cocos2d::GLViewImpl::SetParent((HWND)parent);
	char strBuffer[_MAX_PATH] = { 0, };
	char* pstrBuffer = NULL;

	pstrBuffer = getcwd(strBuffer, _MAX_PATH);


	AppDelegate app;
	cocos2d::Application::getInstance()->run();
}

void Runtime::switchScene()
{
	const auto director = Director::getInstance();

	Scene* newScene = nullptr;

	if (_currentScene++ % 2 == 0)
	{
		newScene = HelloWorld::createScene();
	}
	else
	{
		newScene = GameScene::createScene();
	}

	director->replaceScene(newScene);
}

void Runtime::destroy()
{
	auto director = Director::getInstance();
	director->end();
}

void Runtime::CreateScene()
{
	this->CreateScene(2.0F);
}

//TODO : Create�ÿ��� director pause�ϰ�, Run ������ resume�ϱ�
void Runtime::CreateScene(int speed)
{
	//TODO : Enable to make custom sized Scene;
	Scene* newScene = Scene::createWithSize(Size(640, 640));

	assert(newScene != nullptr && "Failed to create new scene");

	auto gameObjectsLayer = DefaultLayer::CreateDefaultLayerFromJson("ecgsave1.json");

	newScene->addChild(gameObjectsLayer);

	auto director = Director::getInstance();
	director->replaceScene(newScene);

	director->stopAnimation();
	_isAnimationStopped = true;
}

void Runtime::Run()
{
	auto director = Director::getInstance();

	if (_isAnimationStopped)
	{
		director->startAnimation();
		_isAnimationStopped = false;
	}
}


