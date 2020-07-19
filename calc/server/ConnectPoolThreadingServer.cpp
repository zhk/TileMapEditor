#include "StdAfx.h"
#include "ConnectPoolThreadingServer.h"
#include "threads.h"

#define BACKLOG (100)
#define MAX_THR (10)
#define MAX_QUEUE (1000)

static int queue[MAX_QUEUE];
static int head = 0, tail = 0;

static MUTEX_TYPE queue_cs;
static COND_TYPE queue_cv;

void process_queue(void*);	/* multi-threaded request handler for pool */
int	enqueue(SOAP_SOCKET);
SOAP_SOCKET	dequeue();


ConnectPoolThreadingServer::ConnectPoolThreadingServer()
{
}

ConnectPoolThreadingServer::~ConnectPoolThreadingServer()
{
}

ConnectPoolThreadingServer::ConnectPoolThreadingServer(soap_mode imode, soap_mode omode)
{	DataCenterServerService_init(imode, omode);
}


int ConnectPoolThreadingServer::run(int port)
{
	if (soap_valid_socket(bind(NULL, port, 100)))
	{
		server_loop();

		soap_destroy(this);
		soap_end(this);
	}
	else
		return this->error;

	return SOAP_OK;
}

void ConnectPoolThreadingServer::server_loop()
{
	printf("dataCenter is starting...\n");

	MUTEX_SETUP(queue_cs);
	COND_SETUP(queue_cv);

	ConnectPoolThreadingServer* soap_thr[MAX_THR];
	THREAD_TYPE tids[MAX_THR];
	int req;

	int job;
	for (job = 0; job < MAX_THR; job++)
	{
		soap_thr[job] = new ConnectPoolThreadingServer;
		if (!soap_thr[job])
			break;

		soap_copy_context(soap_thr[job], this);

		soap_thr[job]->user = (void*)(long)job; /* int to ptr */

		fprintf(stderr, "Starting thread %d\n", job);
		tids[job] = (HANDLE)_beginthread(process_queue, 8*4096, (void*)soap_thr[job]);
	}

	for (req = 1; ; req++)
	{
		SOAP_SOCKET sock;

		/* to control the close behavior and wait states, use the following:
		soap->accept_flags |= SO_LINGER;
		soap->linger_time = 60;
		*/
		/* the time resolution of linger_time is system dependent, though according
		* to some experts only zero and nonzero values matter.
		*/

		sock = soap_accept(this);
		if (!soap_valid_socket(sock))
		{
			if (this->errnum)
			{
				soap_print_fault(stderr);
				fprintf(stderr, "Retry...\n");
				continue;
			}
			fprintf(stderr, "dataCenter timed out\n");
			break;
		}

		fprintf(stderr, "Request #%d accepted on socket %d connected from IP %d.%d.%d.%d\n", req, sock, (int)(this->ip>>24)&0xFF, (int)(this->ip>>16)&0xFF, (int)(this->ip>>8)&0xFF, (int)this->ip&0xFF);

		while (enqueue(sock) == SOAP_EOM)
			Sleep(1);
	}

	for (job = 0; job < MAX_THR; job++)
	{
		while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
			Sleep(1);
	}

	for (job = 0; job < MAX_THR; job++)
	{
		fprintf(stderr, "Waiting for thread %d to terminate... ", job);
		THREAD_JOIN(tids[job]);
		fprintf(stderr, "terminated\n");
		soap_done(soap_thr[job]);
		delete soap_thr[job];
	}

	COND_CLEANUP(queue_cv);
	MUTEX_CLEANUP(queue_cs);

	printf("dataCenter has ended.\n");
}

void process_queue(void *soap)
{
	ConnectPoolThreadingServer* tsoap = (ConnectPoolThreadingServer*)soap;

	for (;;)
	{
		tsoap->socket = dequeue();
		if (!soap_valid_socket(tsoap->socket))
		{
			fprintf(stderr, "Thread %d terminating\n", (int)(long)tsoap->user);
			break;
		}

		fprintf(stderr, "Thread %d accepted a request\n", (int)(long)tsoap->user);

		if ( tsoap->serve() != SOAP_OK )
		{
			fprintf(stderr, "Thread %d finished serving request with failure %d\n", (int)(long)tsoap->user, tsoap->error);
			tsoap->soap_print_fault(stderr);
		}
		else
		{
			fprintf(stderr, "Thread %d finished serving request\n", (int)(long)tsoap->user);
		}

		soap_destroy(tsoap);
		soap_end(tsoap);
	}

	soap_destroy(tsoap);
	soap_end(tsoap);
	soap_done(tsoap);
}

int enqueue(SOAP_SOCKET sock)
{
	int status = SOAP_OK;
	int next;
	int ret;

	if ((ret = MUTEX_LOCK(queue_cs)))
		fprintf(stderr, "MUTEX_LOCK error %d\n", ret);

	next = (tail + 1) % MAX_QUEUE;
	if (head == next)
	{
		/* don't block on full queue, return SOAP_EOM */
		status = SOAP_EOM;
	}
	else
	{
		queue[tail] = sock;
		tail = next;

		fprintf(stderr, "enqueue(%d)\n", sock);

		if ((ret = COND_SIGNAL(queue_cv)))
			fprintf(stderr, "COND_SIGNAL error %d\n", ret);
	}

	if ((ret = MUTEX_UNLOCK(queue_cs)))
		fprintf(stderr, "MUTEX_UNLOCK error %d\n", ret);

	return status;
}

SOAP_SOCKET dequeue()
{
	SOAP_SOCKET sock;
	int ret;

	if ((ret = MUTEX_LOCK(queue_cs)))
		fprintf(stderr, "MUTEX_LOCK error %d\n", ret);

	while (head == tail)
		if ((ret = COND_WAIT(queue_cv, queue_cs)))
			fprintf(stderr, "COND_WAIT error %d\n", ret);

	sock = queue[head];

	head = (head + 1) % MAX_QUEUE;

	fprintf(stderr, "dequeue(%d)\n", sock);

	if ((ret = MUTEX_UNLOCK(queue_cs)))
		fprintf(stderr, "MUTEX_UNLOCK error %d\n", ret);

	return sock;
}
