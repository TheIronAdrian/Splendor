#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED
#define INFI 100000000
int DEPTH = 8;
#define SNOBNR 4
#define MOD 64
#define ROUNDACCEPTJEWEL 4
#define MAXTIME 2.0f

//#define DEBUG

#include <cassert>
#include <queue>

inline double getTime();

struct MOVE{
  int val,type,rasp[3];
};

struct RET{
  bool isPruned;
  int val;
};

struct BITISETI{
  unsigned long long val[2];
};

int ctzBit(const BITISETI &bit){
  if(bit.val[0]==0){
    return __builtin_ctzll(bit.val[1])+MOD;
  }
  return __builtin_ctzll(bit.val[0]);
}

void addBit(BITISETI &bit, int poz){
  unsigned long long aux;


  if(poz>=MOD){
    poz-=MOD;
    aux = (1LL<<poz);

    bit.val[1]|=aux;
    return;
  }

  aux = (1LL<<poz);
  bit.val[0]|=aux;
}

void rmBit(BITISETI &bit, int poz){
  unsigned long long aux;


  if(poz>=MOD){
    poz-=MOD;
    aux = (1LL<<poz);

    bit.val[1]|=aux;
    bit.val[1]^=aux;
    return;
  }

  aux = (1LL<<poz);

  bit.val[0]|=aux;
  bit.val[0]^=aux;
}

void setZero(BITISETI &bit){
  bit.val[0]=0;
  bit.val[1]=0;
}

bool isZero(const BITISETI &bit){
  if(bit.val[0]==0 && bit.val[1]==0){
    return true;
  }

  return false;
}

BITISETI unite(const BITISETI &a,const BITISETI &b){
  BITISETI aux;

  aux.val[0] = (a.val[0] | b.val[0]);
  aux.val[1] = (a.val[1] | b.val[1]);

  return aux;
}

bool askBit(const BITISETI &bit,int poz){
  unsigned long long aux;


  if(poz>=MOD){
    poz-=MOD;
    aux = (1LL<<poz);

    aux = (aux&bit.val[1]);

    return aux>>poz;
  }
  aux = (1LL<<poz);

  aux = (aux&bit.val[0]);

  return aux>>poz;
}

MOVE ans;

struct DATE{
  BITISETI masa_cards;
  int masa_snob[SNOBNR];
  int masa_gems[GEM_GOLD];
  int player_gems[2][GEM_GOLD];
  int bonus[2][GEM_CNT];
  BITISETI rez[2];
  int nrRez[2];
  int points[2];
  int nrGems[2];
  int nrSnob[2];
  int usedRezerve;

  bool operator==(DATE &a){
    int i,j;


    for(i=1;i<=CARDS_CNT;i++){
      if(askBit(masa_cards,i)!=askBit(a.masa_cards,i)){
        return false;
      }
    }

    for(i=0;i<SNOBNR;i++){
      if(masa_snob[i]!=a.masa_snob[i]){
        return false;
      }
    }

    for(i=0;i<GEM_GOLD;i++){
      if(masa_gems[i]!=a.masa_gems[i]){
        return false;
      }
    }

    for(i=0;i<2;i++){
      for(j=0;j<GEM_GOLD;j++){
        if(player_gems[i][j]!=a.player_gems[i][j]){
          return false;
        }
      }
      for(j=0;j<GEM_CNT;j++){
        if(bonus[i][j]!=a.bonus[i][j]){
          return false;
        }
      }
      for(j=1;j<=CARDS_CNT;j++){
        if(askBit(rez[i],j)!=askBit(a.rez[i],j)){
          return false;
        }
      }

      if(nrRez[i]!=a.nrRez[i]){
        return false;
      }
      if(points[i]!=a.points[i]){
        return false;
      }
      if(nrGems[i]!=a.nrGems[i]){
        return false;
      }
      if(nrSnob[i]!=a.nrSnob[i]){
        return false;
      }
    }

    if(usedRezerve!=a.usedRezerve){
      return false;
    }

    return true;
  }
};

int maxim(int a,int b){
  if(a>b){
    return a;
  }
  return b;
}

int minim(int a,int b){
  if(a<b){
    return a;
  }
  return b;
}

int CalculPersoana(int player,const DATE &game){
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

int CalculStatic(int player,int adan,const DATE &game){
  int s;

  adan = adan;

  s=0;

  s+=CalculPersoana(player,game);
  s-=CalculPersoana(1-player,game);


  return s;
}

int PosibilSnob(int player,DATE &game){
  int x,i,steag,id_snob;

  for(x=0;x<SNOBNR;x++){
    if(game.masa_snob[x]!=-1){
      steag=1;

      id_snob = game.masa_snob[x];

      for(i=0;i<GEM_CNT;i++){
        if(game.bonus[player][i]<NOBLE_CARDS[id_snob][i]){
          steag=0;
          break;
        }
      }


      if(steag==1){
        return x;
      }
    }
  }

  return -1;
}

bool PosibilBuy(int player,int id_card,const DATE &game){
  int i,s;

  s=0;

  for(i=0;i<GEM_CNT;i++){
    s+=maxim(0,CARDS[id_card][i]-(game.bonus[player][i]+game.player_gems[player][i]));
  }

  if(s>game.player_gems[player][GOLD]){
    return false;
  }
  return true;
}

void AddJewel(int player,int jewel,DATE &game){
  game.nrGems[player]++;
  game.masa_gems[jewel]--;
  game.player_gems[player][jewel]++;
}

void RmJewel(int player,int jewel,DATE &game){
  game.nrGems[player]--;
  game.masa_gems[jewel]++;
  game.player_gems[player][jewel]--;
}

void AddJewelMore(int player,int jewel,DATE &game,int cnt){
  game.nrGems[player]+=cnt;
  game.masa_gems[jewel]-=cnt;
  game.player_gems[player][jewel]+=cnt;
}

void RmJewelMore(int player,int jewel,DATE &game,int cnt){
  game.nrGems[player]-=cnt;
  game.masa_gems[jewel]+=cnt;
  game.player_gems[player][jewel]-=cnt;
}

void UpdateAns(int val,int type,int vec0,int vec1,int vec2){
  if(ans.val<val){
    ans.val=val;
    ans.type=type;
    ans.rasp[0]=vec0;
    ans.rasp[1]=vec1;
    ans.rasp[2]=vec2;
  }
}

int CountCards(int player, const DATE &game){
  int i,s;

  s=0;

  for(i=0;i<GEM_CNT;i++){
    s+=game.bonus[player][i];
  }

  return s;
}

bool Prunes(int &alpha,int &beta,int val,int maxer){

  //*
  if(getTime()>=MAXTIME){
    return 1;
  }
  //*/

  if(maxer==1){ ///Maximizeaza

    if(val>=beta){
      return 1;
    }

    alpha = maxim(alpha,val);

    return 0;
  }
  ///Minimizeaza

  if(val<=alpha){
    return 1;
  }

  beta = maxim(beta,val);

  return 0;
}

int Minimax(int player, int adan, DATE &game,int inm,int alpha,int beta);

RET Move1Token(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,aux;

  ///Luam 1 Jetoan
  if(game.nrGems[player]==9){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=1){

        #ifdef DEBUG
        DATE newgame = game;
        #endif // DEBUG

        AddJewel(player,x,game);

        aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

        mi=maxim(mi*inm,aux*inm)*inm;

        if(adan==0){
          UpdateAns(aux,1,x,-1,-1);
        }

        RmJewel(player,x,game);

        #ifdef DEBUG
        assert(newgame==game && "TIP 1 Jeton");
        #endif // DEBUG

        if(Prunes(alpha,beta,aux,inm)){
          return {true,mi};
        }
      }
    }
  }

  return {false,-1};
}

RET Move2Token(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,y,aux;

  ///Luam 2 Jetoane
  if(game.nrGems[player]==8){
    for(x=0;x<GEM_CNT;x++){
      for(y=x+1;y<GEM_CNT;y++){
        if(game.masa_gems[x]>=1 && game.masa_gems[y]>=1){

          #ifdef DEBUG
          DATE newgame = game;
          #endif // DEBUG

          AddJewel(player,x,game);
          AddJewel(player,y,game);

          aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

          mi=maxim(mi*inm,aux*inm)*inm;

          if(adan==0){
            UpdateAns(aux,1,x,y,-1);
          }

          RmJewel(player,x,game);
          RmJewel(player,y,game);

          #ifdef DEBUG
          assert(newgame==game && "TIP 2 Jetoane");
          #endif // DEBUG

          if(Prunes(alpha,beta,aux,inm)){
            return {true,mi};
          }
        }
      }
    }
  }

  return {false,-1};
}

RET Move3Token(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,y,z,aux;

  ///Luam 3 Jetoane
  if(game.nrGems[player]<=7){
    for(x=0;x<GEM_CNT;x++){
      for(y=x+1;y<GEM_CNT;y++){
        for(z=y+1;z<GEM_CNT;z++){
          if(game.masa_gems[x]>=1 && game.masa_gems[y]>=1 && game.masa_gems[z]>=1){

            #ifdef DEBUG
            DATE newgame = game;
            #endif // DEBUG

            AddJewel(player,x,game);
            AddJewel(player,y,game);
            AddJewel(player,z,game);

            aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

            mi=maxim(mi*inm,aux*inm)*inm;

            if(adan==0){
              UpdateAns(aux,1,x,y,z);
            }

            RmJewel(player,x,game);
            RmJewel(player,y,game);
            RmJewel(player,z,game);

            #ifdef DEBUG
            assert(newgame==game && "TIP 3 Jetoane");
            #endif // DEBUG

            if(Prunes(alpha,beta,aux,inm)){
              return {true,mi};
            }
          }
        }
      }
    }
  }
  return {false,-1};
}

RET Move2TokenEgale(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,aux;

  ///Luam 2 Jetoane de aceeasi culoare
  if(game.nrGems[player]<=8){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=4){

        #ifdef DEBUG
        DATE newgame = game;
        #endif // DEBUG

        AddJewel(player,x,game);
        AddJewel(player,x,game);

        aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

        mi=maxim(mi*inm,aux*inm)*inm;

        if(adan==0){
          UpdateAns(aux,2,x,-1,-1);
        }

        RmJewel(player,x,game);
        RmJewel(player,x,game);

        #ifdef DEBUG
        assert(newgame==game && "TIP 2 Jetoane egale");
        #endif // DEBUG

        if(Prunes(alpha,beta,aux,inm)){
          return {true,mi};
        }
      }
    }
  }

  return {false,-1};
}

RET MoveRezerv(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,aux;
  BITISETI iter;

  ///Rezerv
  if(game.nrRez[player]<=2 && (game.nrGems[player]<=9 && game.masa_gems[GOLD]>=1)){
    iter = game.masa_cards;

    while(!isZero(iter)){
      x=ctzBit(iter);
      rmBit(iter,x);

      #ifdef DEBUG
      assert(askBit(game.masa_cards,x)==1 && "Iteratie prin biti failes rezervare");
      DATE newgame = game;
      #endif // DEBUG

      game.nrRez[player]++;
      addBit(game.rez[player],x);
      rmBit(game.masa_cards,x);

      int addGold=0;
      if(game.masa_gems[GOLD]>1){
        addGold=1;

        AddJewel(player,GOLD,game);
      }

      aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

      mi=maxim(mi*inm,aux*inm)*inm;

      if(adan==0){
        UpdateAns(aux,3,x,-1,-1);
      }

      if(addGold==1){
        RmJewel(player,GOLD,game);
      }
      game.nrRez[player]--;
      rmBit(game.rez[player],x);
      addBit(game.masa_cards,x);

      #ifdef DEBUG
      assert(newgame==game && "TIP Rezervare");
      #endif // DEBUG

      if(Prunes(alpha,beta,aux,inm)){
        return {true,mi};
      }
    }
  }

  return {false,-1};
}

RET MoveCumpar(int player, int adan, DATE &game,int inm,int alpha,int beta,int &mi){
  int x,y,aux;
  BITISETI iter;

  ///Cumpar

  iter = unite(game.masa_cards,game.rez[player]);

  while(!isZero(iter)){
    x = ctzBit(iter);
    rmBit(iter,x);

    /*if(x==79){
      printf("nah");
    }*/

    if(PosibilBuy(player,x,game)){

      #ifdef DEBUG
      assert((askBit(game.masa_cards,x)==1 || askBit(game.rez[player],x)==1) && "Iter prin biti failed cumparare");
      DATE newgame = game;
      #endif // DEBUG

      int careDintre;
      int snobAles,snobValoare;


      if(askBit(game.masa_cards,x)==1){
        careDintre=0;
        rmBit(game.masa_cards,x);
      }else{
        careDintre=1;
        rmBit(game.rez[player],x);
      }

      game.points[player]+=CARDS[x][POINTS];

      std::queue <std::pair<int,int>> q;

      for(y=0;y<GEM_CNT;y++){
        aux = maxim(0,CARDS[x][y]-game.bonus[player][y]);

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

      game.bonus[player][CARDS[x][BONUS]]++;

      snobAles = PosibilSnob(player,game);

      if(snobAles!=-1){
        game.nrSnob[player]++;
        game.points[player]+=3;
        snobValoare=game.masa_snob[snobAles];
        game.masa_snob[snobAles]=-1;
      }

      aux = Minimax(1-player,adan+1,game,-inm,alpha,beta);

      mi = maxim(mi*inm,aux*inm)*inm;

      if(snobAles!=-1){
        game.nrSnob[player]--;
        game.points[player]-=3;
        game.masa_snob[snobAles]=snobValoare;
      }

      game.bonus[player][CARDS[x][BONUS]]--;

      if(adan==0){
        UpdateAns(aux,4,x,-1,-1);
      }

      while(!q.empty()){
        AddJewelMore(player,q.front().first,game,q.front().second);
        q.pop();
      }

      game.points[player]-=CARDS[x][POINTS];

      if(careDintre==0){
        addBit(game.masa_cards,x);
      }else{
        addBit(game.rez[player],x);
      }

      #ifdef DEBUG
      assert(newgame==game && "TIP Cumparat");
      #endif // DEBUG

      if(Prunes(alpha,beta,aux,inm)){
        return {true,mi};
      }
    }
  }

  return {false,-1};
}

int Minimax(int player, int adan, DATE &game,int inm,int alpha,int beta){
  int mi;
  RET path;

  //player=id;
  //inm=1;

  mi=INFI*(-inm);

  if(adan==0){
    ans.val=-INFI*10;
  }

  if(adan>0 && (id+player)%2==0 && (game.points[player]>=15 || game.points[1-player]>=15)){
    int calcStatic = CalculStatic(id,adan,game);

    if(game.points[player]==game.points[1-player]){
      int cp,cNp;

      cp = CountCards(id,game);
      cNp= CountCards(1-id,game);

      if(cp==cNp){
        return 0;
      }
      if(cp<cNp){
        return INFI+calcStatic;
      }
      return -INFI+calcStatic;
    }
    if(game.points[id]>game.points[1-id]){
      return INFI+calcStatic;
    }
    return -INFI+calcStatic;
  }

  if(adan>=DEPTH){
    return CalculStatic(id,adan,game);
  }


  ///Cumpar

  path = MoveCumpar(player,adan,game,inm,alpha,beta,mi);
  if(path.isPruned==true){
    return path.val;
  }

  ///Rezerv

  path = MoveRezerv(player,adan,game,inm,alpha,beta,mi);
  if(path.isPruned==true){
    return path.val;
  }

  ///Luam Jetoane
    ///Luam 3 Jetoane
    path = Move3Token(player,adan,game,inm,alpha,beta,mi);
    if(path.isPruned==true){
      return path.val;
    }

    ///Luam 2 Jetoane
    path = Move2Token(player,adan,game,inm,alpha,beta,mi);
    if(path.isPruned==true){
      return path.val;
    }
    ///Luam 1 Jetoan
    path = Move1Token(player,adan,game,inm,alpha,beta,mi);
    if(path.isPruned==true){
      return path.val;
    }

    ///Luam 2 Jetoane Egale
    path = Move2TokenEgale(player,adan,game,inm,alpha,beta,mi);
    if(path.isPruned==true){
      return path.val;
    }

  return mi;
}


#endif // MINIMAX_H_INCLUDED
