#include <functional>

#include "MoveX.h"
#include "RuntimeActionTemplates.h"
#include "ActionArgument.h"

void MoveX::AddActionFromJson(GameObject* targetGameObject, 
						      RuntimeAction* runtimeAction, 
						      const rapidjson::Value& actionValueObject)
{
	const auto& arguments = actionValueObject["Arguments"].GetArray();

	//TODO : ���⵵ ���ø��� �� Ŭ������ ���� �Ű����� ������ ����� �������ϰ� �ϵ��� �ٲ���
	//�� ���ø� ���ο��� Arguments�� �޾Ƽ� ó���ϰų��Ѵ�. �Ʒ��� ���� ���ε��� �Լ��� �� �̸� �����ΰ�

	//Arg1 : Direction
	std::function<void(void)> flipFalse = 
		std::bind(RuntimeActionTemplates::FlipSpriteXFalse, targetGameObject);
	std::function<void(void)> flipTrue = 
		std::bind(RuntimeActionTemplates::FlipSpriteXTrue, targetGameObject);

	const ActionArgument arg1(arguments[0]);

	if (arg1.GetValue().compare("Right") == 0)
	{
		runtimeAction->PushFunctiona(flipTrue);
	}
	else
	{
		runtimeAction->PushFunctiona(flipFalse);
	}

	const ActionArgument arg2(arguments[1]);
	//Value���� generic�̶� string�̱� ����. Ÿ�Ժ��� ��ȯ�����ʿ�����
	int velocity = stoi(arg2.GetValue());
	std::function<void(void)> moveFunc = std::bind(RuntimeActionTemplates::MoveX, targetGameObject, velocity);

	runtimeAction->PushFunctiona(moveFunc);
}
