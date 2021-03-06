#include <rapidjson/document.h>

#include "RuntimeActionTemplates.h"
#include "ActionArgument.h"
#include "RuntimeJumpBy.h"
#include "runtime-core.h"

unique_ptr<RuntimeJumpBy> RuntimeJumpBy::Create(float duration, int height)
{
	auto jumpByAction = make_unique<RuntimeJumpBy>();

	if (jumpByAction && jumpByAction->initWithDuration(duration, Vec2(0, 0), height, 1))
	{
		jumpByAction->autorelease();
		return jumpByAction;
	}

	return nullptr;
}

void RuntimeJumpBy::update(float t)
{
	//TODO : Falldown rightaway when collision occurs on y-axis while jumping up.
	//TODO : Stop falling when collision on y-axis occurs while falling down.

	// parabolic jump (since v0.8.2)
	if (_target)
	{
		float frac = fmodf(t * _jumps, 1.0f);

		float y = (_height) * 4 * frac * (1 - frac);
		//우리는 delta가 어차피 계속 0임
		//y += _delta.y * t;

		float x = _delta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
		Vec2 currentPos = _target->getPosition();

		Vec2 diff = currentPos - _previousPos;
		_startPosition = diff + _startPosition;

		Vec2 newPos = _startPosition + Vec2(x, y);

		//TODO: 여기서 충돌검사를 해야할 듯?
		//여기서 충돌하면, 
		//_elapsed += (1 - 2 * (1 - frac));
		//를 해서 바로 하강으로 이벤트 바꿔야 됨
		_target->setPosition(newPos);

		_previousPos = newPos;

		//TODO: 바닥과 충돌하면 액션 중지하고 y좌표 적절히 세팅
		//_target->stopAction(this);
#else
		_target->setPosition(_startPosition + Vec2(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
	}

}

bool RuntimeJumpBy::initWithDuration(float duration, const Vec2& position, float height, int jumps)
{
	CCASSERT(jumps >= 0, "Number of jumps must be >= 0");
	if (jumps < 0)
	{
		log("JumpBy::initWithDuration error: Number of jumps must be >= 0");
		return false;
	}

	if (ActionInterval::initWithDuration(duration) && jumps >= 0)
	{
		_delta = position;
		_height = height;
		_jumps = jumps;

		return true;
	}

	return false;
}
