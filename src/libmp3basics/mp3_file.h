#ifndef MP3FILE_H
#define MP3FILE_H

#include <string>
#include <map>
#include <list>

#include "mp3_frame.h"

using namespace std;

class MP3File {
  
public:  
  MP3File(string filename);
  ~MP3File();
  string filename();
  
  bool analyze();
  bool ub_analyze();
  
  bool isMP3();
  bool isValid();
  string mpegVersion();  
  unsigned int bitrate();
  unsigned int isVBR();
  unsigned int sampleRate();
  
  bool analyzed();
  unsigned int numFrames();
  unsigned int firstFramePos();
  
  //bool convert(unsigned int target_bitrate);
  
  void debug();
  
private:
  bool m_hasSyncBits(char* buffer, unsigned int offset);
  void m_updateFileInfo(MP3Frame &frame);
  void m_updateFileInfo(list<MP3Frame> &frames);
  
  string m_filename;
  bool m_analyzed;
  string m_mpegVersion;
  unsigned int m_bitrate;
  unsigned int m_sampleRate;
  bool m_isVBR;
  unsigned int m_totalBitrate;
  unsigned int m_totalValidFrames;
  unsigned int m_totalFramesLength;
  unsigned int m_totalFileLength;
  unsigned int m_bytesJunkBefore;
  unsigned int m_bytesJunkBetween;
  unsigned int m_bytesJunkAfter;
  //bool m_converted;
  map<unsigned int, unsigned int> m_framesPerBitrate;
  map<unsigned int, unsigned int> m_framesPerSampleRate;
  
};

#endif
