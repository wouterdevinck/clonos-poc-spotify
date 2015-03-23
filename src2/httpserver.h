/* This file is part of project Clonos - Wouter Devinck
   https://github.com/wouterdevinck/clonos-poc-spotify */

#ifndef H_HTTP_SERVER
#define H_HTTP_SERVER

#include <stdbool.h>

bool HttpListen(int port);
void HttpStop();

void HttpSetPublicKey(char key[0x96]);
void HttpSetName(char name[0x40]);
void HttpSetDeviceID(char id[0x40]);
void HttpSetCallback(void (*callback)(char*, char*, char*));

#endif
