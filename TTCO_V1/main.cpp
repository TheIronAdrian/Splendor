#include <iostream>
#include "Cards.h"
#include "Read.h"
#include "MiniMax.h"
#include "Precalculations.h"
#include "Afisare.h"
using namespace std;

int main(){
  Read_Table();
  CalculatePointsBef();
  CalculateBonusBef();
  CalculateMasks();

  if(round_nr<=4){
    DEPTH=4;
  }else{
    if(round_nr<=6){
      DEPTH=6;
    }
  }

  DATE game = MakeBaseGame();
  Minimax(id,0,game,1,-INFI,INFI);

  Afisare(ans);
  cerr << "kibitz " << ans.val << "\n";

  return 0;
}
