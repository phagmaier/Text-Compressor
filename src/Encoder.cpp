#include "Encoder.h"
#include "Bitio.h"

Encoder::Encoder(const std::string &inputFileName, const std::string &binFileName) 
        : pairs(inputFileName), fileName(binFileName), lower(BOTTOM), upper(MAX){}

void Encoder::encode(){
  std::fstream file(fileName, std::ios::binary | std::ios::out);
  if (!file){
    std::cerr << "COULD NOT OPEN BIN FILE IN ENCODER\n";
    return;
  }
  BitIO bitio(file, false);
  bitio.write_meta_data(pairs);
  unsigned int pending_bits = 0; 
  unsigned int range; 
  for (char c : pairs.text){
    range = upper - lower + 1;
    P pair = pairs.get_pair(c); 
    upper = lower + (range * pair.upper -1) / pairs.total;
    lower = lower + (range * pair.lower) / pairs.total;
    for (;;){
      if (upper < HALF){
        bitio.write_bit(0);
        while (pending_bits > 0){
          bitio.write_bit(1);
          --pending_bits;
        }
      }
      else if(lower >= HALF){
        bitio.write_bit(1);
        while (pending_bits > 0){
          bitio.write_bit(0);
          --pending_bits;
        }
      }
      else if(lower >= QUARTER && upper < THREE_QUARTERS){
        ++pending_bits;
        lower -=QUARTER;
        upper -=QUARTER;
      }
      else break;
      lower = (lower << 1) & MAX;
      upper = ((upper << 1) & MAX) | 1;
    }
  }
  ++pending_bits;
  bitio.write_bit(1);
  while (pending_bits > 0){
    bitio.write_bit(0);
    --pending_bits;
  }
}
