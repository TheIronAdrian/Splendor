#include <iostream>
#include "Cards.h"
#include "Read.h"
#include "MiniMax.h"
#include "Precalculations.h"
#include "Afisare.h"
#include "Time.h"
using namespace std;

#define MAXDEPTH 12

int main(){

  getTime();

  Read_Table();
  CalculatePointsBef();
  CalculateBonusBef();
  CalculateMasks();
  cerr << "kibitz ";

  MOVE lastAns=ans;

  DEPTH=2;
  while(getTime()<=MAXTIME && DEPTH<=MAXDEPTH){
    lastAns=ans;
    DATE game = MakeBaseGame();
    Minimax(id,0,game,1,-INFI,INFI);
    cerr <<"Adancime: "<< DEPTH << " Raspuns: " << ans.val << " Tip: " << ans.type << " ";
    DEPTH++;
  }

  Afisare(lastAns);
  cerr << "FINAL:" << lastAns.val << "\n";

  return 0;
}
