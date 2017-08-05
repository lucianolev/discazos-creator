# Discazos Creator

Last update: February 2013.

A desktop C++ Qt app. Tested only on Linux, but should be portable to other plaforms easily.

This app was made to allow a user from Discazos community (see https://github.com/lucianolev/discazos) to generate an special MP3 file to be shared as an album to Discazos site (external file hosting site indeed).

## What may be interesting from this source code?

Probably not Qt GUI code, but the low-level C++ only MP3 library "libmp3basics". This lib reads and extracts mp3-related info from an MP3 file. It also may serve as a documentation of the format (the MP3 frame).

### Interesting source to look at

**libmp3basics:** https://github.com/lucianolev/discazos-creator/tree/master/src/libmp3basics

**Using the lib to concatenate mp3 files:** https://github.com/lucianolev/discazos-creator/blob/master/src/models/albuminfo.cpp
