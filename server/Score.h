#pragma once
class Score
{
public:
	Score();
	virtual ~Score();
	void incKill();
	void incDeath();
	int getKills() { return kill_num; }
	int getDeath() { return death_num; }
private:
	int kill_num;  // number of kills
	int death_num; // number of death
};

