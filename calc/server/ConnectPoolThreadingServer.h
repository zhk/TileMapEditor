#pragma once

#include "soapDataCenterServerService.h"

class ConnectPoolThreadingServer : public DataCenterServerService
{
public:
	ConnectPoolThreadingServer();
	~ConnectPoolThreadingServer();

	ConnectPoolThreadingServer(soap_mode imode, soap_mode omode);

	virtual	int run(int port);

protected:

	void	server_loop();
};
