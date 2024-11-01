#include "PairDic.h"

PairDic::PairDic(){
  total =0;
  text = "";
}

PairDic::PairDic(const std::string &fileName){
  total = 0;
  std::map<char, unsigned int> temp;
  unsigned int count =0;
  text = "";
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Failed to open file when creating PairDic.\n";
    return;
  }

  char c;
  while (file.get(c)) {  
    text+=c; 
    temp[c] +=1;
    ++total;
  }
  file.close();


  for (auto &i : temp){
    dic[i.first] = P(count, count+i.second);
    count += i.second;
  }
}

P PairDic::get_pair(char c){
  return dic[c];
}

void PairDic::print(){
  std::cout << "Text is: " << text << "\n";
  if (total == text.length()){
    std::cout << "THE TOTAL IS CORRECT IT MATCHES THE STRING LENGTH OF: " << total << "\n"; 
  }
  else{
    std::cout << "THE TOTAL IS NOT CORRECT OUR TOTAL IS: " << total << 
      " BUT THE TEXT LENGTH IS: " << text.length() << "\n";
  }
  for (const auto &[c, pair] : dic){
    std::cout << c << ": [ " << pair.lower << ", " << pair.upper << "]\n";
  }
}
