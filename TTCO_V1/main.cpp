#include <iostream>
#include "Cards.h"
#include "Read.h"
#define MAXTIME 2.0f
#include "MiniMax.h"
#include "Precalculations.h"
#include "Afisare.h"
#include "Time.h"
///TTCO (Think Twice Code Once)
///(a named made by Mocanu Mihai-Adrian and Victor Teodor Nicola)
#define SEED 42
#define MONTECARLOCONST 2.0f

#define CONSTRAND 5


#include "MonteCarlo.h"
using namespace std;

#define MAXDEPTH 12
#define MINIMAX 0
#define MONTECARLO 1
#define TYPEMODEL MINIMAX


int main(){

  std::srand(SEED);

  Read_Table();
  getTime();
  CalculatePointsBef();
  CalculateBonusBef();
  CalculateMasks();

  if(TYPEMODEL == MINIMAX){
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
    cerr << "FINAL: " << lastAns.val << "\n";
  }else{

    nodePool.init();

    DATE baseGame = MakeBaseGame();
    NODE mutare0(-1,{-1,-1,-1,-1},id,baseGame);

    DATE game;
    while(getTime()<=MAXTIME){
      game=baseGame;
      totalSims++;
      GoDeeper(id,0,0,game,mutare0,0);
      //printf("%d %d %d %d %d\n",mutare0.sons.top()->type,mutare0.sons.top()->modi[0],mutare0.sons.top()->modi[1],mutare0.sons.top()->modi[2],mutare0.sons.top()->modi[3]);
      //printf("-----------------------\n");
    }
    double ma=-1;
    MOVE ans;

    while(!mutare0.sons.empty()){
      if(ma<((double)mutare0.sons.top()->wins)/(mutare0.sons.top()->sims)){
        ma=((double)mutare0.sons.top()->wins)/(mutare0.sons.top()->sims);
        ans.type=mutare0.sons.top()->type;
        ans.rasp[0]=mutare0.sons.top()->modi[0];
        ans.rasp[1]=mutare0.sons.top()->modi[1];
        ans.rasp[2]=mutare0.sons.top()->modi[2];
        ans.rasp[3]=mutare0.sons.top()->modi[3];
      }
      mutare0.sons.pop();
    }

    //printf("%d %d %d %d %d\n",mutare0.sons.top()->type,mutare0.sons.top()->modi[0],mutare0.sons.top()->modi[1],mutare0.sons.top()->modi[2],mutare0.sons.top()->modi[3]);

    AfisareMCTS(ans);
    cerr << "kibitz No Move Sims Total: " << NoMovesSims << " \n";
    cerr << "kibitz WINS: " << mutare0.sons.top()->wins << " \n";
    cerr << "kibitz SIMS: " << mutare0.sons.top()->sims << " \n";
    cerr << "kibitz TYPE: " << mutare0.sons.top()->type << " \n";
    cerr << "kibitz VALUE: " << mutare0.sons.top()->value << " \n";
    cerr << "kibitz FINAL: " << ((double)mutare0.sons.top()->wins)/(mutare0.sons.top()->sims) << "\n";
    cerr << "kibitz Total Sims: " << totalSims << "\n";
  }

  return 0;
}
