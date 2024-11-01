#include "Bitio.h"

BitIO::BitIO(std::fstream &fs, bool reading ) : buffer{0}, bits_in_buffer{0}, stream(fs), is_reading{reading}{}

BitIO::~BitIO(){
  if (!is_reading && bits_in_buffer > 0) {
    buffer <<= (CHAR_BITS - bits_in_buffer);
    stream.write(reinterpret_cast<char*>(&buffer), 1);
  }
}

bool BitIO::read_bit() {
  if (bits_in_buffer == 0) {
    stream.read(reinterpret_cast<char*>(&buffer), 1);
    if (stream.eof()) return false;
    bits_in_buffer = CHAR_BITS;
  }
  bool bit = (buffer & 0x80) != 0;
  buffer <<= 1;
  bits_in_buffer--;
  return bit;
}

void BitIO::write_bit(bool bit) {
  buffer = (buffer << 1) | (bit ? 1 : 0);
  bits_in_buffer++;

  if (bits_in_buffer == CHAR_BITS) {
    stream.write(reinterpret_cast<char*>(&buffer), 1);
    buffer = 0;
    bits_in_buffer = 0;
  }
}

void BitIO::write_meta_data(PairDic &ps){
  unsigned int dic_size = ps.dic.size();
  //write the number of characters in the dic
  stream.write(reinterpret_cast<const char*>(&dic_size), sizeof(dic_size));
  //write the total i.e the total characters in the string
  stream.write(reinterpret_cast<const char*>(&ps.total), sizeof(ps.total));
  for (const auto &[key, value] : ps.dic){
    stream.write(reinterpret_cast<const char*>(&key), sizeof(key));
    stream.write(reinterpret_cast<const char*>(&value.upper), sizeof(value.upper));
  }

}

//send in a empty ps
void BitIO::read_meta_data(PairDic &ps){
  char key;
  unsigned int value;
  unsigned int lower = 0;
  unsigned int dic_size;
  stream.read(reinterpret_cast<char*>(&dic_size), sizeof(dic_size));
  stream.read(reinterpret_cast<char*>(&ps.total), sizeof(ps.total));
  
  for (unsigned int i=0; i<dic_size;++i){
   stream.read(reinterpret_cast<char*>(&key), sizeof(key));   
   stream.read(reinterpret_cast<char*>(&value), sizeof(value));   
    ps.dic[key] = P(lower, value);
    lower = value;
  }
  if (lower != ps.total){
    std::cerr << "YOU TOTAL READ DOESN'T MATCH THE TOTAL YOU PUT IN IN READ META DATA\n";
  }
}


