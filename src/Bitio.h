#ifndef BITIO_H
#define BITIO_H

#include <fstream>
#include <string>
#include "PairDic.h"

class BitIO {
private:
  static const int CHAR_BITS = 8;
  unsigned char buffer;
  int bits_in_buffer;
  std::fstream& stream;
  bool is_reading;

public:
  BitIO(std::fstream& fs, bool reading); 
  ~BitIO();
  bool read_bit();
  void write_bit(bool bit);
  void write_meta_data(PairDic &ps);
  void read_meta_data(PairDic &ps);
};
#endif
