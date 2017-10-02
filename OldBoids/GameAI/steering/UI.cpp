#include "UI.h"
#include "Game.h"
#include <string>
#pragma once

UI::UI(ALLEGRO_FONT * pFont, Vector2D position)
{
	mpFont = pFont;
	mPosition = position;
	mMode = VELOCITY;
}

UI::~UI()
{
}

void UI::update()
{
}

void UI::draw()
{
	std::string velocityText =			"[V] Max Velocity: " + std::to_string(static_cast<int>(gpGame->getMaxVelocity()));
	std::string accelerationText =		"[L] Max Acceleration: " + std::to_string(static_cast<int>(gpGame->getMaxAcceleration()));
	std::string angularText =			"[U] Max Angular Velocity: " + std::to_string(static_cast<int>(gpGame->getMaxAngularVelocity()));
	
	std::string cohesionText =			"[C] Cohesion Weight: " + std::to_string(gpGame->getCohesionWeight());
	std::string separationText =		"[S] Separation Weight: " + std::to_string(gpGame->getSeparationWeight());
	std::string alignmentText =			"[A] Alignment Weigth: " + std::to_string(gpGame->getAlignmentWeight());
	std::string velocityMatchingText =	"[M] Velocity Matching Weight: " + std::to_string(gpGame->getVelocityMatchingWeight());
	
	int lineHeight = al_get_font_line_height(mpFont);

	switch (mMode)
	{
	case VELOCITY:
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case ACCELERATION:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case ANGULAR:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case COHESION:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case SEPARATION:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case ALIGNMENT:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	case VELOCITY_MATCHING:
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 0, ALLEGRO_ALIGN_LEFT, velocityText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 1, ALLEGRO_ALIGN_LEFT, accelerationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 2, ALLEGRO_ALIGN_LEFT, angularText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 3, ALLEGRO_ALIGN_LEFT, cohesionText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 4, ALLEGRO_ALIGN_LEFT, separationText.c_str());
		al_draw_text(mpFont, al_map_rgb(255, 255, 255), mPosition.getX(), mPosition.getY() + lineHeight * 5, ALLEGRO_ALIGN_LEFT, alignmentText.c_str());
		al_draw_text(mpFont, al_map_rgb(0, 255, 0), mPosition.getX(), mPosition.getY() + lineHeight * 6, ALLEGRO_ALIGN_LEFT, velocityMatchingText.c_str());
		break;
	}
}