#include "Physics.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->getComponent<CTransform>().pos;
	const auto& p2 = e2->getComponent<CTransform>().pos;
	const auto& bb1 = e1->getComponent<CBoundingBox>();
	const auto& bb2 = e2->getComponent<CBoundingBox>();

	float overlapX = bb2.halfSize.x + bb1.halfSize.x - abs(p2.x - p1.x);
	float overlapY = bb2.halfSize.y + bb1.halfSize.y - abs(p2.y - p1.y);

	return Vec2(overlapX, overlapY);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2)
{
	const auto& p1 = e1->getComponent<CTransform>().prevPos;
	const auto& p2 = e2->getComponent<CTransform>().prevPos;
	const auto& bb1 = e1->getComponent<CBoundingBox>();
	const auto& bb2 = e2->getComponent<CBoundingBox>();

	float overlapX = bb2.halfSize.x + bb1.halfSize.x - abs(p2.x - p1.x);
	float overlapY = bb2.halfSize.y + bb1.halfSize.y - abs(p2.y - p1.y);

	return Vec2(overlapX, overlapY);
}
