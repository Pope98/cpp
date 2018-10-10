#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T>
inline ComponentID GetComponentTypeID() noexcept {
	static ComponentID typeID = GetComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {

public:
	Entity* entity;
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual ~Component();
};

class Entity {
	bool m_active = true;
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void Update() {
		for (auto &c : components)
			c->Update();
		for (auto &c : components)
			c->Draw();
	}
	void Draw() {

	}
	bool isActive() const { return m_active; }
	void destroy() { m_active = false; }
	template<class T>
	bool HasComponent() const {
		return componentBitSet[GetComponentTypeID<T>()];
	}
	template <class T, class... TArgs>
	T& AddComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));
		componentArray[GetComponentTypeID<T>()] = c;
		componentBitSet[GetComponentTypeID<T>()] = true;
		c->Init();
		return *c;
	}
	template<class T>
	T& GetComponent() const {
		auto ptr(componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager {
	std::vector<std::unique_ptr<Entity>> entities;
public:
	void Update() {
		for (auto &e : entities)
			e->Update();
	}
	void Draw() {
		for (auto &e : entities)
			e->Draw();
	}
	void Refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}),
			std::end(entities));
	}
	Entity& AddEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};