#include "LowVoltageCommand.h"
#include <vector>

class DIMServerConfigParser;
class DIMServErrorHandler;

class DIMServerInitialiser
{
public:
	DIMServerInitialiser(void);
	virtual ~DIMServerInitialiser(void);

	
	void StartServer( DIMServerConfigParser* serverConfig );
	void UpdateCommandHandlers( bool updateInDebug );

private:

	std::vector< LowVoltageCommand* > mvec_lowVoltageCommandHandlers;
	DIMServErrorHandler* mp_ErrorHandler;
};

