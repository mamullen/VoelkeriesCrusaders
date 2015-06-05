#pragma once
class GameObject;
class GameLogic;

class Action
{
public:
	Action();
	virtual ~Action();


	virtual void attack(GameObject*){}
	virtual void attack(GameObject*, GameObject*){}
	
	float getDmg() { return c_ad; }
	void setDmg(float d) { c_ad = d; }
	void addDmg(float d) { c_ad += d; }
	void setRange(float m) { c_range = range*m; }
	float getRange() { return c_range; }
	void maxCD() { cd = maxcd; }
	void resetRange() { c_range = range; }
	void resetAd() { c_ad = ad; }
	int getType() { return type; }
	float getCD(){ return cd; }
	float getMaxCD() { return maxcd;  }
	void update() { if (cd > 0){ cd -= 0.1; } }
	
protected:
	int type; // 0 for melee 1 for range, 2 for the basic knock
	float cd;
	float range,c_range;
	float ad,c_ad;
	float maxcd;
};

