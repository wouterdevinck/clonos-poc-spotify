/* This file is part of project Clonos - Wouter Devinck
   https://github.com/wouterdevinck/clonos-poc-spotify */

#ifndef H_HTTP_SERVER
#define H_HTTP_SERVER

#include <stdbool.h>

bool HttpListen(int port);
void HttpStop();

void HttpSetPublicKey(const char* key);
void HttpSetName(const char* name);
void HttpSetDeviceID(const char* id);
void HttpSetCallback(void (*callback)(char*, char*, char*));

#endif