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


  DATE game = MakeBaseGame();
  Minimax(id,0,game,1);

  Afisare(ans);
  cerr << "kibitz " << ans.val << "\n";

  return 0;
}
