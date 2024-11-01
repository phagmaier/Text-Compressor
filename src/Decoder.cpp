#include "Decoder.h"
Decoder::Decoder(const std::string &binFileName) : lower(0), upper(MAX), fileName(binFileName) {}

char Decoder::find_symbol(PairDic &pairs, unsigned int val){
  for (const auto& [c, pair] : pairs.dic){
    if (val >= pair.lower && val < pair.upper){
      return c;
    }
  }
  std::cerr << "ERROR: No Symbol found for value " << val << "\n";
  return '\0';
}

void Decoder::decode(){
  std::fstream file(fileName, std::ios::binary | std::ios::in);
  BitIO bitio(file,true);
  PairDic pairs = PairDic();
  bitio.read_meta_data(pairs);
  unsigned int value = 0;
  unsigned int range;
  unsigned int scaled_value;
  std::string result = "";
  char c;
  P pair;
  //reading intitial value;
  for (unsigned int i=0; i<CODE_VALUE_BITS; ++i){
    value = (value << 1) | bitio.read_bit();
  }

  for (unsigned int i=0; i< pairs.total; ++i){
    range = upper - lower +1;
    scaled_value = ((value - lower) * pairs.total) / range;
    c = find_symbol(pairs, scaled_value);
    if (c == '\0'){
      std::cout << "DECODING FAILED AT POSIION " << i << "\n";
      return;
    }
    result +=c;
    pair= pairs.get_pair(c);
    upper = lower + (range * pair.upper -1) / pairs.total;
    lower = lower + (range * pair.lower) / pairs.total;

    for (;;){
      if (upper < HALF){
        ;
      }
      else if (lower >= HALF){
        value -=HALF;
        lower -=HALF;
        upper -=HALF;
      }
      else if(lower >= QUARTER && upper < THREE_QUARTERS){
        value -= QUARTER;
        lower -= QUARTER;
        upper -= QUARTER;
      }
      else break;
      lower = (lower << 1) & MAX;
      upper = ((upper << 1) & MAX) | 1;
      value = ((value << 1) & MAX) | bitio.read_bit();
    }
  }
  std::cout << "Decoded Text is: " << result << "\n";
}
