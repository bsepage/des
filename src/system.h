#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _4096b  512
#define _192b	24
#define _128b	16
#define _64b	8
#define _56b	7
#define _48b	6
#define _32b	4

#define HANDLE_ERRORNO(err, msg)	\
   	do{ errno = err; perror(msg); exit(EXIT_FAILURE); }while(0)

#define HANDLE_ERROR(msg)   		\
   	do{ perror(msg); exit(EXIT_FAILURE); }while(0)

#endif
