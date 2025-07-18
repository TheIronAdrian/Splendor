#ifndef MONTECARLO_H_INCLUDED
#define MONTECARLO_H_INCLUDED


#include <vector>
#include <math.h>
#include <queue>
#include <array>
int totalSims;

#define NRDATEINNOD 5

int NoMovesSims;

struct CompareNODE;

struct NODE;

// Comparator AFTER NODE is defined
struct CompareNODE {
    bool operator()(const NODE* a, const NODE* b) const;
};

int CalculPersoanaMCTS(int player,const DATE& game){
  int s,i;

  s=0;

  s+=game.points[player]*5;

  for(i=0;i<GEM_CNT;i++){
    s+=game.bonus[player][i]*1;
    s+=game.player_gems[player][i]*1;
  }

  s+=game.nrRez[player]*1;
  s+=game.player_gems[player][GOLD]*1;

  return s;
}

int CalculStaticMCTS(int player,const DATE& game){
  int i,ma;

  ma=-INFI;
  for(i=0;i<nr_players;i++){
    if(i!=player && ma<CalculPersoanaMCTS(i,game)){
      ma=CalculPersoanaMCTS(i,game);
    }
  }

  return CalculPersoanaMCTS(player,game)-ma;
}

struct NODE{
  int type;
  int modi[NRDATEINNOD];
  int sims;
  int wins;
  double value;
  std::priority_queue<NODE*, std::vector<NODE*>, CompareNODE> sons;

  NODE(int a, std::array<int, NRDATEINNOD> b,int player, DATE &game){
    int i;

    type=a;
    sims=0;
    wins=0;

    for(i=0;i<NRDATEINNOD;i++){
      modi[i]=b[i];
    }

    value = INFI+(rand()%20)*CalculPersoanaMCTS(player,game);

    while(!sons.empty()){
      sons.pop();
    }
  }

  void GameUpdate(bool isWon){
    wins+=isWon;
    sims++;

    RevalValue();
  }

  void RevalValue(){
    value = ((double)wins)/sims + MONTECARLOCONST*sqrt(log((double)totalSims)/sims);
  }

  bool operator<(const NODE& other) const {
    return value < other.value;
  }
};

constexpr size_t NODE_POOL_SIZE = 100'000'000;

struct NodePool {
    NODE* pool = nullptr;
    size_t poolIndex = 0;

    void init() {
        pool = static_cast<NODE*>(std::malloc(sizeof(NODE) * NODE_POOL_SIZE));
        poolIndex = 0;
        if (!pool) {
            std::cerr << "Out of memory in NodePool!\n";
            std::exit(1);
        }
    }

    NODE* allocate(const NODE& copyFrom) {
        if (poolIndex >= NODE_POOL_SIZE) {
            std::cerr << "NodePool exhausted!\n";
            std::exit(1); // or return nullptr
        }
        NODE* newNode = &pool[poolIndex++];
        *newNode = copyFrom; // or use placement-new if needed
        return newNode;
    }

    void reset() {
        poolIndex = 0; // reuse the memory in next run
    }

    void destroy() {
        std::free(pool);
        pool = nullptr;
    }
};

NodePool nodePool;

bool CompareNODE::operator()(const NODE* a, const NODE* b) const {
    return a->value < b->value;  // Max heap (highest value at top)
}

int CalcCards(int player,DATE &game){
  int s,i;

  s=0;
  for(i=0;i<GEM_CNT;i++){
    s+=game.bonus[player][i];
  }

  return s;
}

void AddMove1Token(int player,DATE &game,NODE &mutare){
  int x;

  ///Luam 1 Jetoan
  if(game.nrGems[player]==9){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=1){

        NODE copy(1,{1,x,-1,-1},player,game);

        NODE* child = nodePool.allocate(copy);

        mutare.sons.push(child);
      }
    }
  }
}

void AddMove2Token(int player,DATE &game,NODE &mutare){
  int x,y;

  ///Luam 2 Jetoane
  if(game.nrGems[player]==8){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=1){
        for(y=x+1;y<GEM_CNT;y++){
          if(game.masa_gems[y]>=1){
            NODE copy(1,{2,x,y,-1},player,game);

            NODE* child = nodePool.allocate(copy);

            mutare.sons.push(child);
          }
        }
      }
    }
  }
}

void AddMove3Token(int player,DATE &game,NODE &mutare){
  int x,y,z;

  ///Luam 3 Jetoane
  if(game.nrGems[player]<=7){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=1){
        for(y=x+1;y<GEM_CNT;y++){
          if(game.masa_gems[y]>=1){
            for(z=y+1;z<GEM_CNT;z++){
              if(game.masa_gems[z]>=1){
                NODE copy(1,{3,x,y,z},player,game);

                NODE* child = nodePool.allocate(copy);

                mutare.sons.push(child);
              }
            }
          }
        }
      }
    }
  }
}

void AddMove2SameToken(int player,DATE &game,NODE &mutare){
  int x;

  ///Luam 2 Jetoane la fel
  if(game.nrGems[player]<=8){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=4){
        NODE copy(2,{x,-1,-1,-1},player,game);

        NODE* child = nodePool.allocate(copy);

        mutare.sons.push(child);
      }
    }
  }
}

void AddMoveRezervare(int player,DATE &game,NODE &mutare){
  int x;
  BITISETI iter;

  ///Rezerv
  if(game.nrRez[player]<=2 && (game.nrGems[player]<=9 && game.masa_gems[GOLD]>=1)){
    iter = game.masa_cards;

    while(!isZero(iter)){
      x=ctzBit(iter);
      rmBit(iter,x);

      NODE copy(3,{x,-1,-1,-1},player,game);

      NODE* child = nodePool.allocate(copy);

      mutare.sons.push(child);
    }
  }
}

void AddMoveCumparare(int player,DATE &game,NODE &mutare){
  int x;
  BITISETI iter;

  ///Cumpar

  iter = unite(game.masa_cards,game.rez[player]);

  while(!isZero(iter)){
    x = ctzBit(iter);
    rmBit(iter,x);

    if(PosibilBuy(player,x,game)){
      NODE copy(4,{x,-1,-1,-1},player,game);

      NODE* child = nodePool.allocate(copy);

      mutare.sons.push(child);
    }
  }
}

int AddCardInsteadCard(int player,DATE &game,int idCard){
  int mi,ma,idCardNou,aux;

  player=player;

  if(idCard<=40){
    mi=1;
    ma=40;
  }else{
    if(idCard<=70){
      mi=41;
      ma=70;
    }else{
      mi=71;
      ma=90;
    }
  }

  BITISETI iter = game.notPosibleCards;

  iter.val[0]=(~iter.val[0]);
  iter.val[1]=(~iter.val[1]);

  idCardNou=-1;

  do{
    aux=ctzBit(iter);
    if(aux>=mi && aux<=ma && idCard!=aux){
      idCardNou=aux;
    }
    rmBit(iter,aux);
  }while(!isZero(iter) && (idCardNou==-1 || rand()%CONSTRAND==0));


  return idCardNou;
}

bool GoDeeper(int player,int state,int adan,DATE &game,NODE &mutare,int modState){
  int i;

  if(state==nr_players){
    NoMovesSims++;
    return 0;
  }

  if(adan>50*nr_players){
    mutare.GameUpdate(0);

    //std::cerr << "kibitz TO DEEP\n";
    return 0;
  }

  if(adan>0 && player==0){
    int ma,x;

    ma=-1;
    x=-1;
    for(i=0;i<nr_players;i++){
      if(ma<game.points[i]){
        ma=game.points[i];
        x=i;
      }else{
        if(ma==game.points[i] && ma>=15){
          if(CalcCards(x,game)>CalcCards(i,game)){
            x=i;
          }
        }
      }
    }

    if(ma>=15){
      bool isWon;

      isWon=0;
      if(x==id){
        isWon=1;
      }

      mutare.GameUpdate(isWon);

      return isWon;
    }
  }


  if(mutare.sons.empty()){
    ///Generez Mutarile
    modState++;
    AddMoveCumparare(player,game,mutare);
    AddMoveRezervare(player,game,mutare);
    AddMove1Token(player,game,mutare);
    AddMove2Token(player,game,mutare);
    AddMove3Token(player,game,mutare);
    AddMove2SameToken(player,game,mutare);

    if(mutare.sons.empty()){
      mutare.GameUpdate(0);

      NODE copy(1,{0,-1,-1,-1},player,game);

      NODE* child = nodePool.allocate(copy);

      mutare.sons.push(child);

      NODE* mutareNula = mutare.sons.top();
      mutare.sons.pop();

      state++;
      bool isWon=GoDeeper((player+1)%nr_players,state,adan+1,game,*mutareNula,modState);

      mutare.GameUpdate(isWon);
      mutare.sons.push(mutareNula);

      return isWon;
    }
  }

  NODE* nextMove = mutare.sons.top();
  mutare.sons.pop();

  ///Update Gamestate


  int carteNoua = 0,carteVeche = 0;

  int snobAles = 0;

  std::queue <std::pair<int,int>> q;

  while(!q.empty()){
    q.pop();
  }

  if(nextMove->type==1){
    int nrGemLuate = nextMove->modi[0];

    assert(nrGemLuate<=3);

    for(i=1;i<=nrGemLuate;i++){
      AddJewel(player,nextMove->modi[i],game);
    }
  }else if(nextMove->type==2){
    AddJewelMore(player,nextMove->modi[0],game,2);
  }else if(nextMove->type==3){
    carteVeche = nextMove->modi[0];

    game.nrRez[player]++;
    addBit(game.rez[player],carteVeche);
    rmBit(game.masa_cards,carteVeche);

    if(game.masa_gems[GOLD]>1){
      AddJewel(player,GOLD,game);
    }

    carteNoua = AddCardInsteadCard(player,game,carteVeche);
    if(carteNoua!=-1){
      addBit(game.notPosibleCards,carteNoua);
      addBit(game.masa_cards,carteNoua);
    }
  }else{
    carteVeche = nextMove->modi[0];

    if(askBit(game.masa_cards,carteVeche)==1){
      rmBit(game.masa_cards,carteVeche);

      carteNoua = AddCardInsteadCard(player,game,carteVeche);
      if(carteNoua!=-1){
        addBit(game.notPosibleCards,carteNoua);
        addBit(game.masa_cards,carteNoua);
      }
    }else{
      rmBit(game.rez[player],carteVeche);
    }

    game.points[player]+=CARDS[carteVeche][POINTS];



    int y,aux;

    for(y=0;y<GEM_CNT;y++){
      aux = maxim(0,CARDS[carteVeche][y]-game.bonus[player][y]);

      if(aux>0){
        if(aux<game.player_gems[player][y]){
          q.push({y,aux});
          RmJewelMore(player,y,game,aux);
        }else{
          aux-=game.player_gems[player][y];
          q.push({y,game.player_gems[player][y]});
          RmJewelMore(player,y,game,game.player_gems[player][y]);

          q.push({GOLD,aux});
          RmJewelMore(player,GOLD,game,aux);
        }
      }
    }

    game.bonus[player][CARDS[carteVeche][BONUS]]++;

    snobAles = PosibilSnob(player,game);

    if(snobAles!=-1){
      game.nrSnob[player]++;
      game.points[player]+=3;
      game.masa_snob[snobAles]=-1;
    }
  }

  //printf("%d : %d %d %d %d\n",nextMove->type,nextMove->modi[0],nextMove->modi[1],nextMove->modi[2],nextMove->modi[3]);

  bool isWon=GoDeeper((player+1)%nr_players,state,adan+1,game,*nextMove,modState);

  if(modState>=2){
    while(mutare.sons.empty()){
      mutare.sons.pop();
    }
  }

  if(modState<=1){
    mutare.GameUpdate(isWon);
    mutare.sons.push(nextMove);
  }

  return isWon;
}


#endif // MONTECARLO_H_INCLUDED
