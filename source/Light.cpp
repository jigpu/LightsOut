#include "Light.hpp"


Light::Light(int max_state) {
	this->max_state = max_state;
}


int Light::getState() {
	return state;
}


void Light::nextState() {
	state = state >= max_state ? 0 : state + 1;
}
