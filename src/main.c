#include<stdio.h>
#include "spotify.h"

main(int argc, char** argv) {
  char* ver = SpGetLibraryVersion();
  printf("Spotify Connect library version: %s\n", ver);
}
