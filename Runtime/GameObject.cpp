#include <functional>
#include <rapidjson/document.h>

#include "GameObject.h"
#include "KeyEventTrigger.h"
#include "KeyEventTypes.h"
#include "RuntimeActionTemplates.h"

unique_ptr<GameObject> GameObject::CreateFromJson(string filename, int speed)
{
	auto gameObject = make_unique<GameObject>();

	//TODO: Extract these infomation from json file.
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("solbrain.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("solbrain-animations.plist");

	//TODO: Encapsulate them
	gameObject->sprite = Sprite::createWithSpriteFrameName("idle");
	gameObject->sprite->setFlippedX(true);

	gameObject->setScaleFactor(2.0F);

	gameObject->position = Point(10, 2);
	gameObject->size = gameObject->sprite->getContentSize();

	float moveDelta = speed;
	gameObject->addAction("MoveX", &moveDelta);

	return std::move(gameObject);
}

unique_ptr<GameObject> GameObject::CreateFromJsonValue(const rapidjson::Value& value)
{
	auto gameObject = make_unique<GameObject>();

	//TODO: Extract these infomation from json file.
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("solbrain.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("solbrain-animations.plist");

	//TODO: Encapsulate them
	gameObject->sprite = Sprite::createWithSpriteFrameName("idle");
	gameObject->sprite->setFlippedX(true);

	const int scaleFactor = value["ScaleFactor"].GetInt();
	gameObject->setScaleFactor(scaleFactor);

	auto& position = value["Position"];
	gameObject->position = Point(position["X"].GetInt(), position["Y"].GetInt());
	gameObject->size = gameObject->sprite->getContentSize();

	auto& scriptComponents = value["ScriptComponents"];


	for (auto& scriptComponent : scriptComponents.GetArray())
	{
		gameObject->addActionFromJsonValue(scriptComponent);
	}

	//float moveDelta = 2;
	//gameObject->addAction("MoveX", &moveDelta);

	return std::move(gameObject);
}

float GameObject::getScaleFactor() const noexcept
{
	return _scaleFactor;
}

void GameObject::setScaleFactor(float scaleFactor)
{
	_scaleFactor = scaleFactor;
	this->sprite->setScale(_scaleFactor);
}

void GameObject::onUpdate(float delta, const vector<EventKeyboard::KeyCode>& heldKeys)
{
	//TODO: ���� timeDelta�� �ʿ��� �׼��̸� placeholder�� delta���� ä���ѱ��

	//1. handle key events
	for (auto i = _keyboardTriggeredActions.begin(); i != _keyboardTriggeredActions.end(); i++)
	{
		for (auto j = heldKeys.begin(); j != heldKeys.end(); j++)
		{
			const auto context = EventContext(KeyEventType::Down, *j);

			if ((*i)->GetTrigger().IsMatched(context))
			{
				(*i)->execute();
			}
		}
	}
}

//TODO : void* �迭�� ���ڷ� �ޱ� -> �� �׼ǿ� �ʿ��� �Ķ���� ����Ʈ��
void GameObject::addAction(string name, void* param)
{
	if (name == "MoveX")
	{
		/// <param name="keycode">�Ű�� ������ ������ ���ΰ�</param>
		/// <param name="delta">�󸶳� ��������</param>
		assert(param != nullptr && "Param should never be null for MoveX");

		float delta = *((float*)param);

		std::function<void(void)> func = std::bind(RuntimeActionTemplates::MoveX, this, -delta);
		std::function<void(void)> flipFalse = std::bind(RuntimeActionTemplates::FlipSpriteXFalse, this);

		auto trigger = make_unique<KeyEventTrigger>(KeyEventType::Down, EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		auto runtimeAction = make_shared<RuntimeAction>(std::move(trigger));
		runtimeAction->pushFunction(func);
		runtimeAction->pushFunction(flipFalse);

		this->_keyboardTriggeredActions.push_back(runtimeAction);

		trigger = make_unique<KeyEventTrigger>(KeyEventType::Down, EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		std::function<void(void)> func_right = std::bind(RuntimeActionTemplates::MoveX, this, delta);
		std::function<void(void)> flipTrue = std::bind(RuntimeActionTemplates::FlipSpriteXTrue, this);

		runtimeAction = make_shared<RuntimeAction>(std::move(trigger));
		runtimeAction->pushFunction(func_right);
		runtimeAction->pushFunction(flipTrue);

		this->_keyboardTriggeredActions.push_back(runtimeAction);
	}
}

/// <summary>
/// Add action from json 'ScriptComponent' Value
/// </summary>
/// <param name="scriptComponentValue">ScriptComponent rapidjson Value</param>
void GameObject::addActionFromJsonValue(const rapidjson::Value& scriptComponentValue)
{
	auto& triggerValue = scriptComponentValue["Trigger"];
	TriggerType triggerType = static_cast<TriggerType>(triggerValue["Type"].GetInt());


	switch (triggerType)
	{
	case TriggerType::KeyboardEvent:
	{
		auto& keyEventTrigger = triggerValue["KeyEventTrigger"];
		KeyEventType keyEventType = static_cast<KeyEventType>(keyEventTrigger["KeyEventType"].GetInt());
		EventKeyboard::KeyCode keycode = static_cast<EventKeyboard::KeyCode>(keyEventTrigger["Key"].GetInt());


		//TODO : fix this. Ű�ڵ尡 ����̶� �ٸ���.
		if ((int)keycode == 39)
		{
			keycode = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
		}
		else
		{
			keycode = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
		}

		auto trigger = make_unique<KeyEventTrigger>(keyEventType, keycode);

		auto& actionValueList = scriptComponentValue["Actions"];

		auto runtimeAction = make_shared<RuntimeAction>(std::move(trigger));

		for (auto& actionValue : actionValueList.GetArray())
		{
			string actionName = actionValue["Name"].GetString();

			//TODO : Extract this to proprt function
			if (actionName.compare("MoveX") == 0)
			{
				const auto& arguments = actionValue["Arguments"].GetArray();

				//TODO : ���⵵ ���ø��� �� Ŭ������ ���� �Ű����� ������ ����� �������ϰ� �ϵ��� �ٲ���
				//�� ���ø� ���ο��� Arguments�� �޾Ƽ� ó���ϰų��Ѵ�. �Ʒ��� ���� ���ε��� �Լ��� �� �̸� �����ΰ�

				//Arg1 : Direction
				std::function<void(void)> flipFalse = std::bind(RuntimeActionTemplates::FlipSpriteXFalse, this);
				std::function<void(void)> flipTrue = std::bind(RuntimeActionTemplates::FlipSpriteXTrue, this);

				if (strcmp(arguments[0]["Value"].GetString(), "Right") == 0)
				{
					runtimeAction->pushFunction(flipTrue);
				}
				else
				{
					runtimeAction->pushFunction(flipFalse);
				}

				//Value���� generic�̶� string�̱� ����. Ÿ�Ժ��� ��ȯ�����ʿ�����
				string val = arguments[1]["Value"].GetString();
				int velocity = stoi(val);
				std::function<void(void)> moveFunc = std::bind(RuntimeActionTemplates::MoveX, this, velocity);

				runtimeAction->pushFunction(moveFunc);
			}
		}

		this->_keyboardTriggeredActions.push_back(runtimeAction);

		break;
	}
	case TriggerType::ClickEvent:
		break;
	case TriggerType::InteractionEvent:
		break;
	case TriggerType::VariableEvent:
		break;
	default:
		break;
	}
}
