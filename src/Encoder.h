#ifndef ENCODER_H
#define ENCODER_H 

#include "PairDic.h"
#include "Constants.h"
#include "Bitio.h"
#include <fstream>
#include <iostream>

class Encoder{
public:
  Encoder(const std::string &inputFileName, const std::string &binFileName);
  PairDic pairs;
  const std::string fileName;
  unsigned int lower;
  unsigned int upper;
  //std::fstream file;
  //BitIO bitio;
  void encode();

};

#endif
