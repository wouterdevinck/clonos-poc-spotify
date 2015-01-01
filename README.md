Clonos
======
**Open source network audio player**
by Wouter Devinck

Repositories
------------
 * Hardware (block diagram, schematic, pcb layout, ...): <br />
   https://github.com/wouterdevinck/clonos-hardware
 * Software (QT application, GStreamer interaction, ...): <br />
   https://github.com/wouterdevinck/clonos-application
 * Build system (Buildroot configuration files, kernel configuration, ...): <br />
   https://github.com/wouterdevinck/clonos-build
 * Android app: <br />
   https://github.com/wouterdevinck/clonos-android
 * **Efforts to reverse engineer Spotify Connect**: <br />
   https://github.com/wouterdevinck/clonos-poc-spotify

Spotify Connect integration
===========================

**Spotify Connect integration proof-of-concept**

Exports in the library
----------------------
*libspotify\_embedded\_shared.so*

```bash
$ readelf -Ws ../libs/libspotify_embedded_shared.so | grep 'Sp'
     3: 00015b94   164 FUNC    GLOBAL DEFAULT   10 SpRegisterConnectionCallbacks
     4: 0001507c   164 FUNC    GLOBAL DEFAULT   10 SpConnectionLoginPassword
     5: 000159b0   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackPlay
     6: 00015c38   156 FUNC    GLOBAL DEFAULT   10 SpFree
     7: 00014944   220 FUNC    GLOBAL DEFAULT   10 SpPumpEvents
    10: 000148a8   156 FUNC    GLOBAL DEFAULT   10 SpZeroConfGetVars
    12: 0001474c   164 FUNC    GLOBAL DEFAULT   10 SpPlayPreset
    13: 00015398   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackIsRepeated
    15: 00015608   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackUpdateVolume
    19: 00015914   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackPause
    20: 00014cd4   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackSetBitrate
    22: 00015434   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackIsShuffled
    23: 000151c4   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackEnableRepeat
    26: 00015120   164 FUNC    GLOBAL DEFAULT   10 SpConnectionLoginBlob
    29: 00015878   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackSkipToNext
    34: 000154d0   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackIsPlaying
    35: 00014fe0   156 FUNC    GLOBAL DEFAULT   10 SpConnectionLoginOauthToken
    41: 000147f0   184 FUNC    GLOBAL DEFAULT   10 SpConnectionLoginZeroConf
    42: 00015260   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackEnableShuffle
    44: 000146b0   156 FUNC    GLOBAL DEFAULT   10 SpGetPreset
    48: 0001556c   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackGetVolume
    49: 00014a20   156 FUNC    GLOBAL DEFAULT   10 SpSetDisplayName
    56: 00015d70     4 FUNC    GLOBAL DEFAULT   10 SpInit
    58: 00015a4c   164 FUNC    GLOBAL DEFAULT   10 SpRegisterDebugCallbacks
    59: 000152fc   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackIsActiveDevice
    64: 00014b70   192 FUNC    GLOBAL DEFAULT   10 SpGetMetadataImageURL
    65: 00014c30   164 FUNC    GLOBAL DEFAULT   10 SpGetMetadata
    66: 00014ea8   156 FUNC    GLOBAL DEFAULT   10 SpConnectionGetConnectivity
    67: 00014e0c   156 FUNC    GLOBAL DEFAULT   10 SpConnectionLogout
    69: 00015af0   164 FUNC    GLOBAL DEFAULT   10 SpRegisterPlaybackCallbacks
    71: 00014f44   156 FUNC    GLOBAL DEFAULT   10 SpConnectionSetConnectivity
    75: 00014d70   156 FUNC    GLOBAL DEFAULT   10 SpConnectionIsLoggedIn
    79: 00015cd4   156 FUNC    GLOBAL DEFAULT   10 SpGetLibraryVersion
    83: 000157dc   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackSkipToPrev
    84: 000156a4   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackGetPosition
    85: 00014abc   180 FUNC    GLOBAL DEFAULT   10 SpGetMetadataValidRange
    89: 00015740   156 FUNC    GLOBAL DEFAULT   10 SpPlaybackSeek
```

Error codes 
-----------
*As extracted from a decompiled version of libavriradio.so, which uses libspotify\_embedded\_shared.so*

|Code|Name|
|---|---|
|0|SpErrorOk|
|1|SpErrorFailed|
|2|SpErrorInitFailed|
|3|SpErrorWrongAPIVersion|
|4|SpErrorNullArgument|
|5|SpErrorInvalidArgument|
|6|SpErrorUninitialized|
|7|SpErrorAlreadyInitialized|
|8|SpErrorLoginBadCredentials|
|9|SpErrorNeedsPremium|
|10|SpErrorTravelRestriction|
|11|SpErrorApplicationBanned|
|12|SpErrorGeneralLoginError|
|13|SpErrorUnsupported|
|14|SpErrorNotActiveDevice|
|1000|SpErrorPlaybackErrorStart|
|1001|SpErrorGeneralPlaybackError|
|1002|SpErrorPlaybackRateLimited|
|1003|Unknown error|
||SpErrorPlaybackCappingLimitReached|
