#pragma once

#include <string>
#include <tuple>

#include "Components.h"

class EntityManager;

typedef std::tuple <
	CTransform,
	CLifeSpan,
	CInput,
	CBoundingBox,
	CAnimation,
	CGravity,
	CState
> ComponentTuple;

class Entity
{
	friend class EntityManager;

	bool				m_Active = true;
	size_t				m_ID = 0;
	const std::string	m_Tag = "default";
	ComponentTuple		m_Components;

	Entity(const size_t id, const std::string& tag);

public:
	void destroy();
	const size_t id() const;
	bool isActive() const;
	const std::string& tag() const;

	template <typename T>
	bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& getComponent()
	{
		return std::get<T>(m_Components);
	}

	template <typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_Components);
	}

	template <typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}
};

