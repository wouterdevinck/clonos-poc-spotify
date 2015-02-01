/* This file is part of project Clonos - Wouter Devinck
   https://github.com/wouterdevinck/clonos-poc-spotify */

#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "httpserver.h"

#define REPLY_FORMAT "{\"status\": 101, \"statusString\": \"OK\", \"spotifyError\": 0, \"version\": \"2.0.0\", \"deviceID\": \"%s\", \"publicKey\": \"%s\", \"remoteName\": \"%s\", \"activeUser\": \"\"}" 

struct MHD_Daemon* http_daemon;
const char* http_name;
const char* http_public_key;
const char* http_device_id;

char* http_blob;
char* http_client_key;
char* http_user_name;

bool http_blob_received = false;
bool http_client_key_received = false;
bool http_user_name_received = false;

bool http_called_home = false;

void (*http_callback)(char* user_name, char* blob, char* client_key);

void callCallBackIfDone() {
  if (http_blob_received && http_client_key_received && http_user_name_received) {
    if(!http_called_home) {
      http_called_home = true;
      printf("[DEBUG] HTTP: Adding user with blob: %s, clientKey: %s and userName: %s\n", 
        http_blob, http_client_key, http_user_name);
      if(http_callback) http_callback(http_user_name, http_blob, http_client_key);
    }
  }
}

static int iterator(void* cls, enum MHD_ValueKind kind, const char* key, 
 const char* filename, const char* content_type, const char* transfer_encoding, 
 const char* data, uint64_t off, size_t size) {

  //printf("Iterator! %s = %s\n", key, data);
  if(strcmp(key, "blob") == 0){
    // printf("blob - size: %d - off: %d\n", size, off);
    if(off == 0) {
      if(NULL != http_blob) {
        free(http_blob);
      }
      http_blob = malloc(621); 
    }
    strncpy(&http_blob[off], data, size);
    if(off + size == 620) {
      http_blob_received = true;
    }
    callCallBackIfDone();
  } else if (strcmp(key, "clientKey") == 0) {
    // printf("clientKey - size: %d - off: %d\n", size, off);
    if(off == 0) {
      if(NULL != http_client_key) {
        free(http_client_key);
      }
      http_client_key = malloc(129); 
    }
    strncpy(&http_client_key[off], data, size);
    if(off + size == 128) {
      http_client_key_received = true;
    }
    callCallBackIfDone();
  } else if (strcmp(key, "userName") == 0) {
    // printf("userName - size: %d - off: %d\n", size, off);
    if(off == 0) {
      if(NULL != http_user_name) {
        free(http_user_name);
      }
      http_user_name = malloc(strlen(data) + 1); 
    } else {
      printf("[ERROR] HTTP: Did not expect to get the username in multiple pieces!\n");
    }
    strcpy(http_user_name, data);
    http_user_name_received = true;
    callCallBackIfDone();
  } else if (strcmp(key, "action") == 0) {
    if(strcmp(data, "addUser") == 0) { 
      printf("[DEBUG] HTTP: Got POST with action %s\n", data);
    } else {
      printf("[WARNING] HTTP: Got POST with UNKNOWN action %s\n", data);
    }
  } else {
    printf("[WARNING] HTTP: Got POST with UNKNOWN field %s\n", key);
  }
  
  return MHD_YES;

}

static int handleRequest(void* cls, struct MHD_Connection* connection,
 const char* url, const char* method, const char* version,
 const char* upload_data, size_t* upload_data_size, void** ptr) {
  
  struct MHD_PostProcessor *pp = *ptr;
  struct MHD_Response* response;
  char* json;
  int ret;

  if (0 == strcmp(method, "GET")) {
    printf("[DEBUG] HTTP: Incoming GET on %s\n", url);
    if (0 == strcmp(url, "/spotifyconnect")) {;
      const char* action = MHD_lookup_connection_value(
        connection,
        MHD_GET_ARGUMENT_KIND,
        "action"
      );
      if(strcmp(action, "getInfo") == 0){
        printf("[DEBUG] HTTP: GET action: %s\n", action);
        json = malloc(sizeof(char) * (strlen(REPLY_FORMAT) - 6 + strlen(http_device_id) + strlen(http_public_key) + strlen(http_name) + 1));
        sprintf(json, REPLY_FORMAT, http_device_id, http_public_key, http_name);
        printf("[DEBUG] HTTP: Sending GET reply: %s\n", json);
        response = MHD_create_response_from_buffer (strlen (json), (void *) json, MHD_RESPMEM_MUST_FREE);
      } else {
        printf("[WARNING] HTTP: Got GET with UNKNOWN action %s\n", action);
      }
    } else {
      printf("[WARNING] HTTP: Ignoring GET on UNKNOWN url\n");
      return MHD_NO; // fuck this shit, close the connection!
    }
  } else if(0 == strcmp(method, "POST")) {
      
    if (pp == NULL) {
      printf("[DEBUG] HTTP: Incoming POST on %s\n", url);
      pp = MHD_create_post_processor(connection, 65536, &iterator, NULL);
      *ptr = pp;
      return MHD_YES;
    } if (*upload_data_size) {
      http_called_home = false;
      MHD_post_process(pp, upload_data, *upload_data_size);
      *upload_data_size = 0;
      return MHD_YES;
    } else {
      MHD_destroy_post_processor(pp);
      response = MHD_create_response_from_buffer (0, NULL, MHD_RESPMEM_PERSISTENT);
      printf("[DEBUG] HTTP: Finished processing POST\n");
    }
    
  } else {
    printf("[WARNING] HTTP: Ignoring %s request on %s\n", method, url);
    return MHD_NO; // fuck this shit, close the connection!
  }
  
  ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
  return ret;
  
}

bool HttpListen(int port) {
  http_daemon = MHD_start_daemon(
    MHD_USE_SELECT_INTERNALLY,
    port,
    NULL,
    NULL,
    &handleRequest,
    NULL,
    MHD_OPTION_END
  );
  if (http_daemon == NULL) return false;
  return true;
}

void HttpStop() {
  MHD_stop_daemon(http_daemon);
}

void HttpSetPublicKey(const char* key) {
  http_public_key = key;
}

void HttpSetName(const char* name) {
  http_name = name;
}

void HttpSetDeviceID(const char* id) {
  http_device_id = id;
}

void HttpSetCallback(void (*callback)(char*, char*, char*)) {
  http_callback = callback;
}
