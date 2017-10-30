#pragma once

#include "Trackable.h"
#include "Vector2D.h"
#include <allegro5\allegro_font.h>

enum DebugMode
{
	ALL = 0,
	COHESION,
	SEPARATION,
	ALIGNMENT,

	VELOCITY_MATCHING,
	VELOCITY,
	RADIUS,
	ANGULAR,
	ACCELERATION,
};

class UI : public Trackable
{
private:
	ALLEGRO_FONT* mpFont;
	Vector2D mPosition;
	DebugMode mMode;

public:
	UI(ALLEGRO_FONT* pFont, Vector2D position);
	~UI();

	inline void setMode(DebugMode mode) { mMode = mode; };
	inline DebugMode getMode() { return mMode; };
	void update();
	void draw();
};