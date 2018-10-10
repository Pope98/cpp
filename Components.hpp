#pragma once

#include "ECS.hpp"

class PositionComponent : public Component {
	int m_xpos;
	int m_ypos;
public:
	int X() { return m_xpos; }
	int Y() { return m_ypos; }
	void Init() override {
		m_xpos = 0;
		m_ypos = 0;
	}
	void Update() override {
		m_xpos++;
		m_ypos++;
	}
	void SetPos(int x, int y) {
		m_xpos = x;
		m_ypos = y;
	}

};