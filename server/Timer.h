#pragma once
#include "ConfigSettings.h"
class Timer
{
public:
	Timer();
	virtual ~Timer();

	void reset() { time = 0; }
	virtual void update(int t) {
		time += t; 
		if (round < atof((ConfigSettings::config->getValue("NumResets").c_str())) && time >= max) { round++; reset(); printf("reset\n"); }
	}
	int getTime() { return time; }
	int getState();
	void setMaxTime(int n) { max = n; } // maximum time of one day night cycle
	
	void print() { printf("Time =  %d\n", time); }
	void setPhase(int, int, int); //set time period for day/night/day
protected:
	int round; // # of rounds happened for day/night/day cycle
	int time;
	int max;
	int p1, p2, p3; // time period for day/night/day
};

