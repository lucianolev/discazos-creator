#ifndef MP3FRAME_H
#define MP3FRAME_H

#include <bitset>
#include <string>

using namespace std;

class MP3Frame {

public:
  MP3Frame(char header32bits[]);
  bool isValid(unsigned int target_bitrate = 0);
  string mpegVersion();
  unsigned int bitrate();
  unsigned int sampleRate();
  unsigned int samplesPerFrame();
  unsigned int length();
  
  void debugHeader();

private:
  bool m_isPadded();
  
  bitset<32> m_header;

};

#endif
