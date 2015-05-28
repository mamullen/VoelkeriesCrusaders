#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual float getDmg() = 0;    // get attack damage
	virtual float getRange() = 0;  // get attack range
	virtual void setRange(float) = 0; // modify attack range 

	void maxCD() { cd = maxcd; }
	void resetRange() { c_range = range; }
	void resetAd() { c_ad = ad; }

	float getCD(){ return cd; }
	void update() { if (cd > 0){ cd -= 0.1; } }
	
protected:
	float cd;
	float range,c_range;
	float ad,c_ad;
	float maxcd;
};

