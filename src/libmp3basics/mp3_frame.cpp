#include <iostream>

#include "mp3_frame.h"

/*
 MPEG Header Format (32 bits)
 ----------------------------
 
 11111111 11111111 11111111 11111111
 SSSSSSSS SSSVVLLE BBBBFFPU UUUUUUUU
 
 S (11 bits) = Sync bits
 V (2 bits)  = Version
 L (2 bits)  = Layer
 E (1 bit)   = Error Protection bit
 B (4 bits)  = Bitrate
 F (2 bits)  = Sample Rate Frecuency
 P (1 bit)   = Padding bit
 U (9 bits)  = Unimportant bits (for this class task!)
*/

MP3Frame::MP3Frame(char header32bits[]) {
  bitset<32> bits(header32bits[0]); //Initialize in last byte
  //Save the other 3 bytes (in reverse order)
  for(int bc = 1; bc < 4; bc++) {
    bits <<= 8;
    bits |= (unsigned char)header32bits[bc];
  }
  
  m_header = bits;
}

//Valid: MPEG Layer III
bool MP3Frame::isValid(unsigned int target_bitrate) {
  //Sync bits are not checked because this is 
  //used when the frame has already been found
  
  //Check that is a valid MPEG Version (!= 01)
  if(!m_header[20] && m_header[19])
    return false;
  
  //Check that is MPEG Layer III (= 01)
  if(m_header[18] || !m_header[17])
    return false;
  
  //Error protection bit is always valid
  
  //Check valid bitrate (=! 1111 and =! 0000)
  if((m_header[15] && m_header[14] && m_header[13] && m_header[12]) || 
     (!m_header[15] && !m_header[14] && !m_header[13] && !m_header[12]))
    return false;
  //If specified, check target bitrate
  if(target_bitrate != 0 && this->bitrate() != target_bitrate)
    return false;
  
  //Check sample rate is valid (!= 11)
  if(m_header[11] && m_header[10])
    return false;
  
  //Padding bit is always valid
    
  return true;
}

string MP3Frame::mpegVersion() {
  bitset<2> mpegVersion_bits(0);
  mpegVersion_bits.set(0, m_header[19]);
  mpegVersion_bits.set(1, m_header[20]);
  unsigned long mpegVersion_index = mpegVersion_bits.to_ulong();  

  static const string mpegVersions[4] = { "2.5", "0", "2", "1" };  
  
  return mpegVersions[mpegVersion_index];
}

unsigned int MP3Frame::bitrate() {
  bitset<4> bitrate_bits(0);
  for(int i = 0; i < 4; i++) {
    bitrate_bits.set(i, m_header[12+i]); 
  }
  unsigned long bitrate_index = bitrate_bits.to_ulong();
  
  static const int v1_bitrates[16] = 
    { 0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 0 };
  static const int v2_bitrates[16] =
    { 0, 8,  16, 24, 32,  40,  48,  56,  64,  80,  96,  112, 128, 144, 160, 0 };
    
  if(this->mpegVersion() == "1") {
    return v1_bitrates[bitrate_index];
  } else if(this->mpegVersion() == "2" || this->mpegVersion() == "2.5") {
    return v2_bitrates[bitrate_index];
  } else {
    return 0;
  }
}

unsigned int MP3Frame::sampleRate() {
  bitset<2> samplerates_bits(0);
  samplerates_bits.set(0, m_header[10]);
  samplerates_bits.set(1, m_header[11]);
  unsigned long samplerate_index = samplerates_bits.to_ulong();
  
  static const int v1_samplerates[4] = { 44100, 48000, 32000, 0 };
  static const int v2_samplerates[4] = { 22050, 24000, 16000, 0 };
  static const int v25_samplerates[4] = { 11025, 12000, 8000, 0 };
  
  if(this->mpegVersion() == "1") {
    return v1_samplerates[samplerate_index];
  } else if(this->mpegVersion() == "2") {
    return v2_samplerates[samplerate_index];
  } else if(this->mpegVersion() == "2.5") {
    return v25_samplerates[samplerate_index];
  } else {
    return 0;
  }
}

unsigned int MP3Frame::samplesPerFrame() {
    unsigned int samplesPerFrame = 0;
    if(this->mpegVersion() == "1") {
      samplesPerFrame = 1152;
    } else if(this->mpegVersion() == "2" || this->mpegVersion() == "2.5") {
      samplesPerFrame = 576;
    }
    return samplesPerFrame;
}

unsigned int MP3Frame::length() {
  if(!this->isValid()) {
    return 0;
  }
  
  int length = this->samplesPerFrame()*(1000*this->bitrate())/(8*this->sampleRate());
  if(this->m_isPadded())
    length++;
  
  return length;
}

void MP3Frame::debugHeader() {
  cout << "DEBUG - Header: ";
  for(int i = 0; i < 32; i++) {
    if(i % 8 == 0) cout << " ";
    cout << m_header[31-i];
  }
  cout << endl;
}

bool MP3Frame::m_isPadded() {
  return m_header[9];
}
