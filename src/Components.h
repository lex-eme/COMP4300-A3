#pragma once

#include "Vec2.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

struct Component
{
	bool has = false;
};

struct CTransform : public Component
{
	Vec2 pos		= { 0.0f, 0.0f };
	Vec2 prevPos	= { 0.0f, 0.0f };
	Vec2 scale		= { 4.0f, 4.0f };
	Vec2 velocity	= { 0.0f, 0.0f };
	float angle		= 0.0f;

	CTransform() {}
	CTransform(const Vec2& p)
		: pos(p) {}
	CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

struct CLifeSpan : public Component
{
	int lifespan = 0;
	int frameCreated = 0;

	CLifeSpan() {}
	CLifeSpan(int duration, int frame)
		: lifespan(duration), frameCreated(frame) {}
};

struct CInput : public Component
{
	bool up			= false;
	bool down		= false;
	bool left		= false;
	bool right		= false;
	bool shoot		= false;
	bool canShoot	= true;
	bool canJump	= true;

	CInput() {}
};

struct CBoundingBox : public Component
{
	Vec2 size;
	Vec2 halfSize;

	CBoundingBox() {}
	CBoundingBox(const Vec2& s)
		: size(s), halfSize(s.x / 2, s.y /2) {}
};

struct CAnimation : public Component
{
	Animation animation;
	bool repeat = false;

	CAnimation() {}
	CAnimation(const Animation& anim, bool r)
		: animation(anim), repeat(r) {}
};

struct CGravity : public Component
{
	float gravity = 0.0f;

	CGravity() {}
	CGravity(float g)
		: gravity(g) {}
};

struct CState : public Component
{
	std::string state = "jumping";

	CState() {}
	CState(const std::string& s)
		: state(s) {}
};