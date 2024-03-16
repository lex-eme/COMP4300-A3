#include "Physics.h"

static Vec2 overlap(const Vec2& p1, const Vec2& p2, const Vec2& s1, const Vec2& s2)
{
	float overlapX = s2.x + s1.x - abs(p2.x - p1.x);
	float overlapY = s2.y + s1.y - abs(p2.y - p1.y);

	return Vec2(overlapX, overlapY);
}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->getComponent<CTransform>().pos;
	const auto& p2 = e2->getComponent<CTransform>().pos;
	const auto& bb1 = e1->getComponent<CBoundingBox>().halfSize;
	const auto& bb2 = e2->getComponent<CBoundingBox>().halfSize;
	return overlap(p1, p2, bb1, bb2);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->getComponent<CTransform>().prevPos;
	const auto& p2 = e2->getComponent<CTransform>().prevPos;
	const auto& bb1 = e1->getComponent<CBoundingBox>().halfSize;
	const auto& bb2 = e2->getComponent<CBoundingBox>().halfSize;
	return overlap(p1, p2, bb1, bb2);
}
