#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "signals.h"
#include "opennopd.h"
#include "logger.h"
#include "fetcher.h"
#include "worker.h"

void signal_handler(int sig) 
{
	char message [LOGSZ];

	switch(sig){
		case SIGHUP:
			sprintf(message, "Received SIGHUP signal.");
			logger(LOG_INFO, message);
			break;
		case SIGTERM:
			sprintf(message, "Received SIGTERM signal.");
			logger(LOG_INFO, message);
			
			switch(servicestate){
				case RUNNING:
					servicestate = STOPPING;
					fetcher_graceful_exit();
					shutdown_workers();
					break;
				case STOPPING:
					servicestate = STOPPED;
					break;
				default:
					servicestate = STOPPED;
					break;
			}
			
			break;
		case SIGQUIT:
			sprintf(message, "Received SIGQUIT signal.");
			logger(LOG_INFO, message);
			break;
		default:
			sprintf(message, "Unhandled signal (%s)", strsignal(sig));
			logger(LOG_INFO, message);
			break;
	}
}

