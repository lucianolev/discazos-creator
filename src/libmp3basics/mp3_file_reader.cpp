#include "mp3_file_reader.h"

#define HEADER_SIZE     4

MP3FileReader::MP3FileReader(MP3File* file) {
  if(!file->analyzed()) {
    file->analyze();
  }
  m_fileStream = new ifstream(file->filename().c_str(), ios::in|ios::binary);
  m_fileStream->seekg(file->firstFramePos());
  m_totalFrames = file->numFrames();
  m_framesRead = 0;
  m_lastReadCount = 0;
  m_currentFrameData = NULL;
}

MP3FileReader::~MP3FileReader() {
  if(m_currentFrameData != NULL) {
    delete[] m_currentFrameData;
  }
  delete m_fileStream;
}

char* MP3FileReader::readNextFrame() {
  if(eof()) {
    return NULL;
  }
  if(m_currentFrameData != NULL) {
    delete[] m_currentFrameData;
  }
  
  char nextFrameHeader[HEADER_SIZE];
  m_fileStream->read(nextFrameHeader, HEADER_SIZE);
  m_fileStream->seekg(-HEADER_SIZE, ios_base::cur);
  
  MP3Frame nextFrame(nextFrameHeader);
  char* m_currentFrameData = new char[nextFrame.length()];
  m_fileStream->read(m_currentFrameData, nextFrame.length());
  m_framesRead++;
  m_lastReadCount = nextFrame.length();
  
  return m_currentFrameData;
}

unsigned int MP3FileReader::lastReadCount() {
  return m_lastReadCount;
}

bool MP3FileReader::eof() {
  return (m_totalFrames == m_framesRead);
}
