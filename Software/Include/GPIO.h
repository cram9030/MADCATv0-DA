#ifndef GPIO_H_
#define GPIO_H_
#include <string>
#include <fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

typedef int (*CallbackType)(int);

class GPIO {
public:
	enum DIRECTION{INPUT,OUTPUT};
	enum VALUE{LOW=0,HIGH=1};
	enum EDGE{NONE,RISING,FALLING,BOTH};

private:
	int number, debounceTime;
	string name, path;

public:
	GPIO(int number);
	virtual int getNumber() {return number;}

	virtual int setDirection(GPIO::DIRECTION);
	virtual GPIO::DIRECTION getDirection();
	virtual int setValue(GPIO::VALUE);
	virtual int toggleOutput();
	virtual GPIO::VALUE getValue();
	virtual int setActiveLow(bool isLow=true);
	virtual int setActiveHigh();
	//software debounce input (ms) - default 0
	virtual void setDebounceTime(int time) {this->debounceTime = time;}

	virtual int streamOpen();
	virtual int streamWrite(GPIO::VALUE);
	virtual int streamClose();

	virtual int toggleOutput(int time);
	virtual int toggleOutput(int numerOfTimes, int time);
	virtual void changeToggleTime(int time) {this->togglePeriod = time;}
	virtual void toggleCancel() {this->threadRunning = false;}

	virtual int setEdgeType(GPIO::EDGE);
	virtual GPIO::EDGE getEdgeType();
	virtual int waitForEdge();
	virtual int waitForEdge(CallbackType callback);
	virtual void waitForEdgeCancel() {this->threadRunning = false;}

	virtual ~GPIO();

private:
	int exportGPIO();
	int unexportGPIO();
	ofstream stream;
	pthread_t thread;
	CallbackType callbackFunction;
	bool threadRunning;
	int togglePeriod, toggleNumber;
	friend void *threadedPoll(void *value);
	friend void *threadedToggle(void *value);
};

void *threadedPoll(void *value);
void *threadedToggle(void *value);

#endif