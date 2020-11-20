#pragma once
#include <map>

#include "MoveX.h"

#define REGISTER_FUNC_TYPE function<void(GameObject*, RuntimeAction* runtimeAction, const rapidjson::Value&)>

class RuntimeActionFactoryConatiner
{
public:
	static void Register(string actionName, REGISTER_FUNC_TYPE actionFactory);

private:
	//TODO : �̸� - ��� �Լ� �� ���⿡ ����ؼ� �׳� �ٷ� �Լ� �̸��� Ű�� �� �� �ְ� �ٲٱ�
	static std::map<string, REGISTER_FUNC_TYPE> _actionList;
};