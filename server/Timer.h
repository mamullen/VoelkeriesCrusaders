#pragma once
class Timer
{
public:
	Timer();
	virtual ~Timer();

	void reset() { time = 0; }
	void update(int t) { time += t; }
	int getTime() { return time; }
	int getState();
	void print() { if(time%1000 == 0) printf("Time =  %d\n", time); }
	void setPhase(int, int, int); //set time period for day/night/day
private:
	int time;
	int p1, p2, p3; // time period for day/night/day
};

