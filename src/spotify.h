/* This file is part of project Clonos - Wouter Devinck
   https://github.com/wouterdevinck/clonos-poc-spotify */

#ifndef H_SPOTIFY
#define H_SPOTIFY

#include <stdbool.h>

/* NOTE: This header was reverse engineered. This turned out to be quite a
 * bit harder than engineering in the right direction, so it may contain 
 * errors or be incomplete. Not everything was tested. */

typedef struct {
  int version;
  void* buffer;
  int bufferSize;
  void* appKey;
  int appKeySize;
  const char* deviceId;
  const char* displayName;
  char* brand;
  char* model;
  bool magicBool1; // TODO What is this?
  int (*onError)(unsigned int, int, int, int);
  bool magicBool2; // TODO What is this?
  int (*onPlaybackNotify)(int a1, int a2, int a3, int a4);
  int (*onPlaybackAudioData)();
  int (*onPlaybackSeek)(int a1, int a2, int a3, int a4);
  int (*onPlaybackApplyVolume)(int volume);
  int (*onConnectionNotify)(int a1, int a2, int a3, int a4);
  int (*onConnectionNewCredentials)(const char *a1, int a2, int a3, int a4);
  void (*onConnectionMessage)(int a1, int a2, int a3, int a4);
} SpInitSettings;

// Utilities
const char* SpGetLibraryVersion();

// Settings
int SpSetDisplayName(const char* name); 

// Plumbing - TODO not sure how these work exactly
void SpPumpEvents(); // TODO does this work?
void SpFree();
int SpInit(SpInitSettings* settings); 

// Callbacks

typedef struct {
  char* (*DebugMessage)(char*, int);
} SpDebugCallbacks;

typedef struct {
  int (*PlaybackNotify)(int a1, int a2, int a3, int a4);
  int (*PlaybackAudioData)();
  int (*PlaybackSeek)(int a1, int a2, int a3, int a4);
  int (*PlaybackApplyVolume)(int volume);
} SpPlaybackCallbacks;

typedef struct {
  int (*ConnectionNotify)(int a1, int a2, int a3, int a4);
  int (*ConnectionNewCredentials)(const char *a1, int a2, int a3, int a4);
  void (*ConnectionMessage)(int a1, int a2, int a3, int a4);
} SpConnectionCallbacks;

int SpRegisterDebugCallbacks(SpDebugCallbacks* callbacks, int unknown);  
int SpRegisterPlaybackCallbacks(SpPlaybackCallbacks* callbacks, int unknown); 
int SpRegisterConnectionCallbacks(SpConnectionCallbacks* callbacks, int unknown); 

// Status
bool SpPlaybackIsActiveDevice();
bool SpPlaybackIsRepeated();
bool SpPlaybackIsShuffled();
bool SpPlaybackIsPlaying();
int SpPlaybackGetVolume();
int SpPlaybackGetPosition();

// Control - TODO return bool?
void SpPlaybackSeek(unsigned int position);
void SpPlaybackSetBitrate(unsigned int bitrate);
void SpPlaybackEnableRepeat(bool enable);
void SpPlaybackEnableShuffle(bool enable);
void SpPlaybackUpdateVolume(int volume);
void SpPlaybackSkipToPrev();
void SpPlaybackSkipToNext();
void SpPlaybackPause();
void SpPlaybackPlay();

// Presets - TODO no clue what this is for
void SpGetPreset(signed int a1, int a2);
void SpPlayPreset(int a1, int a2);

// Metadata - TODO return types may be wrong
void SpGetMetadataValidRange(int a1, int a2);
void SpGetMetadataImageURL(int a1, unsigned int a2, void *a3, int a4);
void SpGetMetadata(int a1, int a2);

// Zeroconf
int SpZeroConfGetVars(const char* key);

// Connection status - TODO not entirely sure how to use this
bool SpConnectionGetConnectivity();
void SpConnectionSetConnectivity(bool connected); // TODO parameter is a guess

// Login
void SpConnectionLoginZeroConf(char* username, char* blob, char* clientkey);
void SpConnectionLoginOauthToken(char* token);
void SpConnectionLoginPassword(char* username, char* password);
void SpConnectionLoginBlob(char* username, char* blob);

// Login status and logout
bool SpConnectionIsLoggedIn();
void SpConnectionLogout();

#endif