#include <fstream>
#include <iostream>
#include <bitset>

#include "mp3_file.h"

#define BUFFER_SIZE                 8196

#define MIN_VALID_FRAMES            10
#define MIN_CONSECUTIVE_1ST_FRAMES  3
#define MAX_BYTES_JUNK_BEFORE       4096
#define MAX_BYTES_JUNK_BETWEEN      0
#define MAX_PERCENTAGE_JUNK_AFTER   10

MP3File::MP3File(string filename) {
  m_filename = filename;
  m_analyzed = false;
  m_mpegVersion = "0";
  m_bitrate = 0;
  m_sampleRate = 0;
  m_isVBR = false;
  m_totalBitrate = 0;
  m_totalValidFrames = 0;
  m_totalFramesLength = 0;
  m_totalFileLength = 0;
  m_bytesJunkBefore = 0;
  m_bytesJunkBetween = 0;
  m_bytesJunkAfter = 0;
  //m_converted = false;
}

MP3File::~MP3File() {
  //if(m_converted) {
    //delete file m_filename;
  //}
}

string MP3File::filename() {
  return m_filename;
}

//Unbuffered analyze
bool MP3File::ub_analyze() {
  ifstream fileStream(m_filename.c_str(), ios::in|ios::binary);
  if(!fileStream.is_open()) {
    //Throw exception
    cout << "Error opening input file!" << endl; 
    return false;
  }
  
  //Get file length
  fileStream.seekg (0, ios::end);
  m_totalFileLength = fileStream.tellg();
  fileStream.seekg (0, ios::beg);
  
  char* file_buff = new char[m_totalFileLength];
  fileStream.read(file_buff, m_totalFileLength);
  unsigned int global_pos = 0;
  bool frameFound;
  list<MP3Frame> posible1stFrames;
  bool firstFrameFound = false;
  while(global_pos < m_totalFileLength) {
    frameFound = false;
    if(m_hasSyncBits(file_buff, global_pos)) {
      char header32bits[4] = 
        { file_buff[global_pos], file_buff[global_pos+1], file_buff[global_pos+2], file_buff[global_pos+3] };
      MP3Frame frame(header32bits);
      if(frame.isValid()) {
        frameFound = true;
        if(firstFrameFound) {
          this->m_updateFileInfo(frame);
        } else {
          posible1stFrames.push_back(frame);
          if(posible1stFrames.size() == MIN_CONSECUTIVE_1ST_FRAMES) {
            firstFrameFound = true;
            this->m_updateFileInfo(posible1stFrames);
            posible1stFrames.clear();
          }
        }
        global_pos += frame.length();
      }
    }
    if(!frameFound) {
      if(firstFrameFound) {
        m_bytesJunkBetween += 1;
        m_bytesJunkAfter += 1;
      } else {
        if(posible1stFrames.size() > 0) {
          list<MP3Frame>::iterator itFrame;
          for(itFrame = posible1stFrames.begin(); itFrame != posible1stFrames.end(); ++itFrame) {
            global_pos -= itFrame->length();
          }
          posible1stFrames.clear();
        }
        m_bytesJunkBefore += 1;
      }
      global_pos += 1;
    }
  }
  delete[] file_buff;
  
  m_bytesJunkBetween -= m_bytesJunkAfter;
  
  if(m_isVBR) {
    m_bitrate = m_totalBitrate / m_totalValidFrames;
  }
  fileStream.close();
  m_analyzed = true;
  
  return true;  
}

bool MP3File::analyze() {
  ifstream fileStream(m_filename.c_str(), ios::in|ios::binary);
  if(!fileStream.is_open()) {
    //Throw exception
    cout << "Error opening input file!" << endl; 
    return false;
  }
  
  //Get file length
  fileStream.seekg (0, ios::end);
  m_totalFileLength = fileStream.tellg();
  fileStream.seekg (0, ios::beg);
  
  char* buffer;
  const int buffer_size = BUFFER_SIZE;
  int buff_pos = 0;
  unsigned int diff_buff_size;
  bool frameFound;
  list<MP3Frame> posible1stFrames;
  bool firstFrameFound = false;
  while(!fileStream.eof()) {
    diff_buff_size = buff_pos - fileStream.gcount();
    if(diff_buff_size > 0) {
      fileStream.seekg(diff_buff_size, ios_base::cur);
    }
    buffer = new char[buffer_size];
    fileStream.read(buffer, buffer_size);
    //Process the buffer
    buff_pos = 0;
    while(buff_pos < fileStream.gcount()) {
      frameFound = false;
      if(m_hasSyncBits(buffer, buff_pos)) {
        char header32bits[4] = 
          { buffer[buff_pos], buffer[buff_pos+1], buffer[buff_pos+2], buffer[buff_pos+3] };
        MP3Frame frame(header32bits);
        if(frame.isValid()) {
          frameFound = true;
          if(firstFrameFound) {
            this->m_updateFileInfo(frame);
          } else {
            posible1stFrames.push_back(frame);
            if(posible1stFrames.size() == MIN_CONSECUTIVE_1ST_FRAMES) {
              firstFrameFound = true;
              this->m_updateFileInfo(posible1stFrames);
              posible1stFrames.clear();
            }
          }
          buff_pos += frame.length();
        }
      }
      if(!frameFound) {
        if(firstFrameFound) {
          m_bytesJunkBetween += 1;
          m_bytesJunkAfter += 1;
        } else {
          m_bytesJunkBefore += 1;
          if(posible1stFrames.size() > 0) {
            list<MP3Frame>::iterator itFrame;
            int remainingBuffer = fileStream.gcount() - buff_pos;
            int rewind_total = -remainingBuffer;
            for(itFrame = posible1stFrames.begin(); itFrame != posible1stFrames.end(); ++itFrame) {
              rewind_total -= itFrame->length();
            }
            posible1stFrames.clear();
            rewind_total += 1;
            fileStream.seekg(rewind_total, ios_base::cur);
            buff_pos = fileStream.gcount();
          }
        }
        buff_pos += 1;
      }
    }
    delete[] buffer;
  }
  m_bytesJunkBetween -= m_bytesJunkAfter;
  
  if(m_isVBR) {
    m_bitrate = m_totalBitrate / m_totalValidFrames;
  }
  fileStream.close();
  m_analyzed = true;
  
  return true;
}

bool MP3File::isMP3() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }

  return (m_totalValidFrames > MIN_VALID_FRAMES);
}

bool MP3File::isValid() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  if(!isMP3()) {
    //Throw Exception
    cout << "El archivo no es un MP3 válido!" << endl;
  }
  
  float percentageJunkAfter = (m_bytesJunkAfter * 100)/m_totalFileLength;
  
  bool littleJunkBefore = (m_bytesJunkBefore <= MAX_BYTES_JUNK_BEFORE);
  bool noBetweenJunk = (m_bytesJunkBetween <= MAX_BYTES_JUNK_BETWEEN);
  bool lowJunkAfter = (percentageJunkAfter <= MAX_PERCENTAGE_JUNK_AFTER);
  
  return (littleJunkBefore && noBetweenJunk && lowJunkAfter);
}

string MP3File::mpegVersion() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }

  return m_mpegVersion;
}

unsigned int MP3File::bitrate() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  
  return m_bitrate;
}

unsigned int MP3File::isVBR() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  return m_isVBR;
}

unsigned int MP3File::sampleRate() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  return m_sampleRate;
}

unsigned int MP3File::samplesPerFrame() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  return m_samplesPerFrame;
}

unsigned int MP3File::lengthInMs() {
    if(!m_analyzed) {
      //Throw Exception
      cout << "El archivo no fue analizado!" << endl;
    }

    unsigned int lengthInMs = (this->samplesPerFrame() / (this->sampleRate() / 1000.0)) *
                               this->numFrames();
    return lengthInMs;
}

bool MP3File::analyzed() {
  return m_analyzed;
}

unsigned int MP3File::numFrames() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }
  
  return m_totalValidFrames;
}

unsigned int MP3File::firstFramePos() {
  if(!m_analyzed) {
    //Throw Exception
    cout << "El archivo no fue analizado!" << endl;
  }

  return m_bytesJunkBefore;
}

void MP3File::debug() {
  cout << "File: " << filename() << endl;
  cout << "Length: " << m_totalFileLength << " bytes" << endl;
  cout << "---------------------" << endl;
  cout << "Is MP3? " << (isMP3() ? "yes" : "no") << endl;
  cout << "Is valid? " << (isValid() ? "yes" : "no") << endl;
  cout << "Valid frames: " << m_totalValidFrames << " frames" << endl;
  cout << "Frames length: " << m_totalFramesLength << " bytes" << endl;
  cout << "---------------------" << endl;
  cout << "MPEG Version: " << mpegVersion() << endl;
  cout << "Bitrate: " << bitrate() << " kbps" << (isVBR() ? " (VBR)" : "") << endl;
  cout << "Sample rate: " << m_sampleRate << " hz" << endl;
  cout << "---------------------" << endl;
  cout << "Junk before: " << m_bytesJunkBefore << " bytes" << endl;
  cout << "Junk between: " << m_bytesJunkBetween << " bytes" << endl;
  cout << "Junk after: " << m_bytesJunkAfter << " bytes" << endl;
  cout << "---------------------" << endl;
  cout << "First frame position: " << firstFramePos() << endl;
  cout << "Frames per bitrate:" << endl;
  map<unsigned int, unsigned int>::const_iterator itFpb;
  for(itFpb = m_framesPerBitrate.begin(); itFpb != m_framesPerBitrate.end(); ++itFpb) {
    cout << "-- " << itFpb->first << " kbps => " << itFpb->second << " frames" << endl;
  }
  cout << "Frames per sample rate:" << endl;
  map<unsigned int, unsigned int>::const_iterator itFpsr;
  for(itFpsr = m_framesPerSampleRate.begin(); itFpsr != m_framesPerSampleRate.end(); ++itFpsr) {
    cout << "-- " << itFpsr->first << " hz => " << itFpsr->second << " frames" << endl;
  }
}

bool MP3File::m_hasSyncBits(char* buffer, unsigned int offset) {
  if(buffer[offset] == static_cast<char>(0xFF)) {
    bitset<8> secondByte(buffer[offset+1]);
    // check to see if the second byte matches 111xxxxx
    return secondByte.test(7) && secondByte.test(6) && secondByte.test(5);    
  }
  return false;
}

void MP3File::m_updateFileInfo(MP3Frame &frame) {
  m_totalValidFrames += 1;
  m_totalFramesLength += frame.length();
            
  if(!m_isVBR && m_bitrate != 0 && m_bitrate != frame.bitrate()) {
    m_isVBR = true;
  }
  m_mpegVersion = frame.mpegVersion(); //File mpegVersion = frame mpegVersion
  m_bitrate = frame.bitrate(); //File bitrate = frame bitrate
  m_sampleRate = frame.sampleRate(); //File samplerate = frame samplerate
  m_samplesPerFrame = frame.samplesPerFrame(); //File samples per frame = frame samples per frame
  m_totalBitrate += frame.bitrate(); //For VBR calculation
  m_bytesJunkAfter = 0; //Reset junkAfter if frame is valid

  m_framesPerBitrate[frame.bitrate()] += 1; //DEBUG
  m_framesPerSampleRate[frame.sampleRate()] += 1; //DEBUG  
}

void MP3File::m_updateFileInfo(list<MP3Frame> &frames) {
  list<MP3Frame>::iterator itFrame;
  for(itFrame = frames.begin(); itFrame != frames.end(); ++itFrame) {
    this->m_updateFileInfo(*itFrame);
  }
}
