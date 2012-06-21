#ifndef MP3FILE_READER_H
#define MP3FILE_READER_H

#include <fstream>

#include "mp3_file.h"

using namespace std;

class MP3FileReader {

public:
  MP3FileReader(MP3File* file);
  ~MP3FileReader();
  char* readNextFrame();
  unsigned int lastReadCount();
  bool eof();
  
private:
  ifstream* m_fileStream;
  char* m_currentFrameData;
  unsigned int m_lastReadCount;
  unsigned int m_totalFrames;
  unsigned int m_framesRead;

};

#endif
