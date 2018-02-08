#pragma once
class Alive {
	// This is any object that has to be updated every frame, even if it doesn't move

public:
	virtual ~Alive() {}
	virtual bool beat() { return false; };
};