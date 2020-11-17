#pragma once
#include <vector>
#include <rapidjson/document.h>

#include "cocos2d.h"
#include "RuntimeAction.h"

using namespace std;
using namespace cocos2d;

class GameObject : public Node
{
public:
	static unique_ptr<GameObject> CreateFromJson(string filename, int speed=4);
	static unique_ptr<GameObject> CreateFromJsonValue(const rapidjson::Value& value);
	Point position;
	Size size;

	//TODO: Encapsulate all sprite and animation related fields to seperate component ex) Visual
	Sprite* sprite;

	float getScaleFactor() const noexcept;
	void setScaleFactor(float scaleFactor);

	virtual void onUpdate(float delta, const vector<EventKeyboard::KeyCode>& heldKeys);

private:
	//TODO: ������ key�� �����ϰ� map���� ������ �ٲ�ȿ�� ���̱�
	//TODO : change to smart pointers
	std::vector<shared_ptr<RuntimeAction>> _keyboardTriggeredActions;

	void addAction(string name, void* param);

	//TODO : this is temporal
	void addActionFromJsonValue(const rapidjson::Value& value);

	float _scaleFactor = 1.0F;
};

