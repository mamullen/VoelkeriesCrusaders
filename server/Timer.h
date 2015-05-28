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
	void print() { printf("Time =  %d\n", time); }
private:
	int time;
};

