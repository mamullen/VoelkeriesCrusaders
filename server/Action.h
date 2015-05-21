#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual float getDmg() = 0;    // get attack damage
	virtual float getRange() = 0;  // get attack range
	virtual void maxCD() = 0;

	float getCD(){ return cd; }
	void update() { if (cd > 0){ cd -= 0.1; } }
	
protected:
	float cd;
};

