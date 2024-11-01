#include <iostream>
//#include "PairDic.h"
#include <string>
#include "Encoder.h"
#include "Decoder.h"

int main(){
  std::string textFileName = "../data/text.txt";
  std::string binFileName = "../data/binfile";
  Encoder encoder = Encoder(textFileName, binFileName);
  encoder.encode();
  Decoder decoder = Decoder(binFileName);
  decoder.decode();

  return 0;
}
