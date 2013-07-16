#include "EventTimer.h"
#include "LowVoltageCommand.h"
#include <iostream>
EventTimer::EventTimer( int timeTilEvt ) : m_evtTime(timeTilEvt) 
{
}


EventTimer::~EventTimer(void)
{
}

void EventTimer::StartTimer(){

	this->start(m_evtTime);
	std::cout<<"Starting"<<std::endl;

}

void EventTimer::SetEvtObject( LowVoltageCommand* pointerToActionObject ){

	mp_LVControlObj = pointerToActionObject;

}
void EventTimer::timeHandler( bool debug ){

	std::cout<<"Checking PS"<<std::endl;
	mp_LVControlObj->Update( debug );
	this->StartTimer();

}