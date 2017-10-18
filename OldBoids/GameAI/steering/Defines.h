#pragma once

#include <string>

typedef int IDType;
typedef std::string IDDescriptor;

const IDType INVALID_ID = -1;

const IDType HIGHEST_ID = 9999;

#define GRAPHICS_SYSTEM gpGame->getGraphicsSystem()