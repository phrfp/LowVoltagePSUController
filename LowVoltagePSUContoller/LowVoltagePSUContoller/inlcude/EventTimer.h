#pragma once
#include <dis.hxx>

class LowVoltageCommand;

class EventTimer : public DimTimer
{
public:
	EventTimer( int timeTilEvt );
	virtual ~EventTimer(void);
	void timeHandler();
	void StartTimer();
    void SetEvtObject( LowVoltageCommand* pointerToActionObject );

private:
	int m_evtTime;
	LowVoltageCommand *mp_LVControlObj;
};

