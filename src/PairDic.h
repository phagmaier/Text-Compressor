#ifndef PAIRDIC_H
#define PAIRDIC_H
#include <map>
#include <string>
#include <fstream>
#include "P.h"
#include <iostream>

class PairDic{
public:
  PairDic();
  PairDic(const std::string &fileName);
  P get_pair(char c);
  std::map<char, P> dic;
  unsigned int total;
  std::string text;
  void print();
};


#endif
