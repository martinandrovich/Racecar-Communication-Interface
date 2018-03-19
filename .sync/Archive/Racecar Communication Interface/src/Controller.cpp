#include "Controller.h"
#include "Common.h"


Controller::Controller()
{
}


Controller::~Controller()
{
}


const auto& Controller::GetData(int _variable) const
{
	return this->velocity;
}