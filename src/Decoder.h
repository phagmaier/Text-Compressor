#ifndef DECODER_H
#define DECODER_H
#include <string>
#include "PairDic.h"
#include "Constants.h"
#include "Bitio.h"
#include <string>

class Decoder{
public:
  Decoder(const std::string &binFileName);
  unsigned int lower;
  unsigned int upper;
  const std::string fileName;
  void decode();
  char find_symbol(PairDic &ps, unsigned int value);
};

#endif
