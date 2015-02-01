/* This file is part of project Clonos - Wouter Devinck
   https://github.com/wouterdevinck/clonos-poc-spotify */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "spotify.h"
#include "httpserver.h"

#define BUFFER_SIZE 1048576 // 1 MB
#define HTTP_PORT 8080

const char* player_name = "Living room";
const char* device_id = "8c:a9:82:5b:9f:8c";

const char APPLICATION_KEY[] = {
  0x01, 0xD9, 0x47, 0x78, 0x3C, 0x37, 0x66, 0x2D, 0x45, 0x18, 0x78, 0x43, 0x6B, 0xEB, 0x8B, 0xE1,
  0x43, 0x3F, 0x75, 0xA0, 0xF1, 0xF8, 0xE2, 0x80, 0xCD, 0xA7, 0xD9, 0xF7, 0xE9, 0xB5, 0x1D, 0x3C,
  0x57, 0x2C, 0xC2, 0xC1, 0x35, 0x01, 0x62, 0xD5, 0x24, 0xF6, 0x3F, 0x1F, 0xAE, 0x51, 0x04, 0x79,
  0xAC, 0xDF, 0xDD, 0x44, 0xC9, 0x29, 0xED, 0xC5, 0x9A, 0x2D, 0xE6, 0x8E, 0x25, 0x46, 0xB6, 0x9D,
  0x0D, 0xA3, 0xCB, 0x64, 0x23, 0xC0, 0xCA, 0x14, 0x99, 0xD2, 0xF9, 0xD8, 0xB3, 0xE9, 0x34, 0x52,
  0x97, 0x6E, 0x6D, 0x43, 0x6A, 0x54, 0x74, 0x82, 0xCD, 0xC0, 0x9B, 0xE7, 0xC2, 0xC8, 0x12, 0xF5,
  0xDC, 0x71, 0x76, 0x75, 0xDC, 0xCF, 0x85, 0x88, 0x19, 0xDA, 0x05, 0xD4, 0x77, 0x06, 0xAB, 0xE7,
  0x9D, 0xC4, 0x29, 0xD5, 0x2D, 0x51, 0xA0, 0x85, 0x9D, 0xC1, 0x41, 0x8A, 0xAC, 0x06, 0xF2, 0x2A,
  0xBB, 0x3A, 0x81, 0x2B, 0x60, 0x46, 0x4C, 0xD1, 0x8D, 0x65, 0xBC, 0xF7, 0x44, 0x8E, 0x9F, 0x4E,
  0x34, 0x0D, 0x5D, 0xC6, 0x1F, 0x67, 0xB2, 0x14, 0x20, 0xDD, 0x13, 0x12, 0x88, 0x63, 0x88, 0x0E,
  0x6B, 0x34, 0xC6, 0x02, 0x90, 0xDB, 0x60, 0x32, 0xD4, 0xBA, 0xC3, 0x8E, 0x74, 0x1E, 0x80, 0x8D,
  0x27, 0xA6, 0x42, 0x4F, 0x45, 0x9F, 0x3C, 0x73, 0xB8, 0x70, 0xD7, 0xE2, 0x6F, 0x99, 0x08, 0x40,
  0x52, 0xAA, 0xFC, 0x27, 0x84, 0x68, 0xF0, 0xF6, 0x21, 0x25, 0xD5, 0xC4, 0xAB, 0x2D, 0x0C, 0xB8,
  0x01, 0x78, 0x52, 0x7E, 0xA5, 0xDE, 0x53, 0x25, 0x0F, 0x6C, 0x1D, 0x96, 0xBC, 0x4F, 0x34, 0xE0,
  0xDD, 0x44, 0xD8, 0x87, 0xA3, 0x70, 0xDC, 0x79, 0xE4, 0xAF, 0xAE, 0xC6, 0xF3, 0xE5, 0x57, 0xB3,
  0xEF, 0xE3, 0x87, 0xC7, 0x3F, 0x9C, 0x70, 0x50, 0xF3, 0x16, 0x3A, 0x1F, 0x3E, 0x1E, 0xE5, 0xE0,
  0x60, 0x2C, 0xDF, 0x4F, 0x98, 0x60, 0x29, 0x52, 0xAB, 0xFB, 0x65, 0xD3, 0x38, 0x33, 0xE6, 0xB2,
  0x10, 0xF0, 0xA2, 0xE1, 0xB8, 0xBB, 0x5B, 0x9B, 0x04, 0x76, 0x94, 0xD5, 0xCF, 0xFC, 0x32, 0xD9,
  0xAE, 0x3F, 0x4D, 0x8D, 0x33, 0x64, 0x05, 0x5A, 0xD0, 0xE4, 0xAD, 0xBC, 0x77, 0x11, 0xD6, 0x13,
  0xED, 0xE6, 0xC8, 0x2E, 0x7A, 0x53, 0x50, 0x0B, 0xBD, 0x50, 0x27, 0x5D, 0x62, 0x48, 0x50, 0x8C,
  0x05
};

//char* MAGIC = "5mWFE9t5h4jayeEWmIC561ygybl44TGrKoFaHRvFTDKkdmCkL1RT7ZRys7DLfis2G5Hcl1bgRkd586QZlnK5cOdEERXjdTiORwpyAncepWO6S5esnX0JTGXdG9m7npB3c6jhgK1jlwVxwTdWV9co2B9IGnilMGy71vBbSDGjgji7dRnvFXq0K7ORbJPYQDCtfnw4LK7X7vYqy8z7tu9xUhlAqVXR0ZVkeWqXUn29I02AUpfn2srVC6hnCW92EZ38vFUnOeIiXZbZ5KUnpzoQ7qKm3FQmjVw0BSRXE0KrFERTLZETwkBRSIxCOhHNL5SQ";

/*int debug(unsigned int a1, int a2) {
  printf("DEBUG\n");
  return 0;
}*/

char* onDebugMessage(char* message, int unknown) {
  printf("[DEBUG] %s\n", message);
  return message;
}

/*int callback(unsigned int a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
  return 0;
}*/

int onError(unsigned int a1, int a2, int a3, int a4) {
  printf("ERROR!\n");
  return 0;
}

int onPlaybackNotify(int a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
  return 0;
}

int onPlaybackAudioData() {
  printf("CALLBACK\n");
  return 0;
}

int onPlaybackSeek(int a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
  return 0;
}

int onPlaybackApplyVolume(int volume) {
  printf("CALLBACK\n");
  return 0;
}

int onConnectionNotify(int a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
  return 0;
}

int onConnectionNewCredentials(const char *a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
  return 0;
}

void onConnectionMessage(int a1, int a2, int a3, int a4) {
  printf("CALLBACK\n");
}

void onZeroconfSignIn(char* user_name, char* blob, char* client_key) {
  SpConnectionLoginZeroConf(user_name, blob, client_key);
  // SpConnectionLoginBlob(user_name, blob);
}

main() {
  SpInitSettings* settings;
  SpDebugCallbacks* debugcbs;
  SpPlaybackCallbacks* playbackcbs;
  SpConnectionCallbacks* connectioncbs;
  const char* ver;
  int r;
  int pid;
  int pid2;
  const char* publicKey;
  int res;
  
  HttpListen(HTTP_PORT);  
  HttpSetName(player_name);
  HttpSetDeviceID(device_id);
  HttpSetCallback(&onZeroconfSignIn);
  
  /*publicKey = malloc(2);
  strcpy((char*)publicKey, "e");
  HttpSetPublicKey(publicKey);
  
  getc(stdin);
  return;*/

  pid = fork();

  if (pid == 0) {

    execlp(
      "avahi-publish",
      "avahi-publish",
      "-s",
      player_name,
      "_spotify-connect._tcp",
      "8080", // HTTP_PORT, //- TODO
      "CPath=/spotifyconnect", // TODO put in define in header
      "VERSION=1.0",
      NULL
    );

  } else {
    
    settings = malloc(sizeof *settings);

    settings->version = 4;
    settings->buffer = malloc(BUFFER_SIZE);
    settings->bufferSize = BUFFER_SIZE;
    settings->appKey = &APPLICATION_KEY;
    settings->appKeySize = sizeof(APPLICATION_KEY);
    settings->deviceId = device_id;
    settings->displayName = player_name;
    settings->brand = "BluOS"; // TODO - "ONKYO";
    settings->model = "N100"; // TODO - "TXNR708";
    settings->magicBool1 = false;
    settings->onError = onError;
    settings->magicBool2 = false;
    settings->onPlaybackNotify = onPlaybackNotify;
    settings->onPlaybackAudioData = onPlaybackAudioData;
    settings->onPlaybackSeek = onPlaybackSeek;
    settings->onPlaybackApplyVolume = onPlaybackApplyVolume;
    settings->onConnectionNotify = onConnectionNotify;
    settings->onConnectionNewCredentials = onConnectionNewCredentials;
    settings->onConnectionMessage = onConnectionMessage;

    printf("\n _____             _   _  __         _____                             _   \n");
    printf("/  ___|           | | (_)/ _|       /  __ \\                           | |  \n");
    printf("\\ `--. _ __   ___ | |_ _| |_ _   _  | /  \\/ ___  _ __  _ __   ___  ___| |_ \n");
    printf(" `--. \\ '_ \\ / _ \\| __| |  _| | | | | |    / _ \\| '_ \\| '_ \\ / _ \\/ __| __|\n");
    printf("/\\__/ / |_) | (_) | |_| | | | |_| | | \\__/\\ (_) | | | | | | |  __/ (__| |_ \n");
    printf("\\____/| .__/ \\___/ \\__|_|_|  \\__, |  \\____/\\___/|_| |_|_| |_|\\___|\\___|\\__|\n");
    printf("      | |                     __/ |                                        \n");
    printf("      |_|                    |___/                                         \n\n");

    debugcbs = malloc(sizeof *debugcbs);
    debugcbs->DebugMessage = onDebugMessage;
    printf("SpRegisterDebugCallbacks: %d\n", SpRegisterDebugCallbacks(debugcbs, 0));

    ver = SpGetLibraryVersion();
    printf("Spotify Connect library version: %s\n", ver);

    res = SpInit(settings);
    printf("SpInit: %d\n", res);
    if(res != 0) exit(1);

    playbackcbs = malloc(sizeof *playbackcbs);
    playbackcbs->PlaybackNotify = onPlaybackNotify;
    playbackcbs->PlaybackAudioData = onPlaybackAudioData;
    playbackcbs->PlaybackSeek = onPlaybackSeek;
    playbackcbs->PlaybackApplyVolume = onPlaybackApplyVolume;
    printf("SpRegisterPlaybackCallbacks: %d\n", SpRegisterPlaybackCallbacks(playbackcbs, 0));

    connectioncbs = malloc(sizeof *connectioncbs);
    connectioncbs->ConnectionNotify = onConnectionNotify;
    connectioncbs->ConnectionNewCredentials = onConnectionNewCredentials;
    connectioncbs->ConnectionMessage = onConnectionMessage;
    printf("SpRegisterConnectionCallbacks: %d\n", SpRegisterConnectionCallbacks(connectioncbs, 0));

    printf("Is playing: %d, is active device: %d, is repeated: %d, is shuffled: %d, volume: %d, position: %d\n",
      SpPlaybackIsPlaying(),
      SpPlaybackIsActiveDevice(),
      SpPlaybackIsRepeated(),
      SpPlaybackIsShuffled(),
      ((SpPlaybackGetVolume() * 100) / 65535),
      SpPlaybackGetPosition()
    );

    printf("Is logged in: %d\n", SpConnectionIsLoggedIn());

    printf("Set name: %d\n", SpSetDisplayName(player_name));

    publicKey = malloc(129); // TODO - always? 
    printf("ZeroConf: %d\n", SpZeroConfGetVars(publicKey));
    printf("PublicKey: %s\n", publicKey);
    HttpSetPublicKey(publicKey);
    
    printf("Connected: %d\n", SpConnectionGetConnectivity());
    //SpConnectionSetConnectivity(false); 
    //printf("Connected: %d\n", SpConnectionGetConnectivity());

    while (true) {
      SpPumpEvents();
      //if (r != 0) {
      //  printf("BOEM! %d\n", r);
      //}
      sleep(1); // TODO - this fails?
    }
    
    getc(stdin);

  }
  
}