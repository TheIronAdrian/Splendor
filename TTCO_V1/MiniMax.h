#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED
#define INFI 100000000
#define DEPTH 4
#include <queue>

struct MOVE{
  int val,type,rasp[3];
};

MOVE ans;

struct DATE{
  int masa_cards[CARDS_CNT+1];
  int masa_gems[GEM_GOLD];
  int player_gems[2][GEM_GOLD];
  int bonus[2][GEM_CNT];
  int rez[2][CARDS_CNT+1];
  int nrRez[2];
  int points[2];
  int nrGems[2];
  int usedRezerve;
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
  int s,i,j,cont[5],nr,sumGem[5];

  s=0;
  s+=game.points[player]*10000;

  for(i=0;i<GEM_CNT;i++){
    s+=game.bonus[player][i]*50*(15-round_nr);
  }

  sumGem[0]=0;
  sumGem[1]=0;
  sumGem[2]=0;
  sumGem[3]=0;
  sumGem[4]=0;

  for(i=1;i<=CARDS_CNT;i++){
    if(game.rez[player][i]==1 || game.masa_cards[i]==1){
      for(j=0;j<GEM_CNT;j++){
        sumGem[CARDS[i][j]]+=CARDS[i][POINTS];
      }
    }
    if(game.rez[player][i]==1){
      s+=CARDS[i][POINTS]*5000;
      cont[CARDS[i][BONUS]]++;
    }
  }

  for(j=0;j<GEM_CNT;j++){
    //s+=(sumGem[j]*game.player_gems[player][j])/15;
  }

  cont[0]=0;
  cont[1]=0;
  cont[2]=0;
  cont[3]=0;
  cont[4]=0;
  for(i=0;i<GEM_CNT;i++){
    if(game.bonus[player][i]>0){
      cont[i]++;
    }
  }

  nr=0;
  for(i=0;i<GEM_CNT;i++){
    if(cont[i]==1){
      nr++;
    }
  }

  if(nr<=3){
    s+=20000;
  }


  s-=game.nrRez[player]*(5-round_nr)*50;
  s+=game.player_gems[player][GOLD]*3;

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

int Minimax(int player, int adan, DATE &game,int inm){
  int x,y,z,mi,aux;

  //player=id;
  //inm=1;

  mi=INFI*(-inm);

  if(adan==0){
    ans.val=-INFI*10;
  }

  if(adan>=DEPTH){
    return CalculStatic(id,adan,game);
  }

  if(adan>0 && (id+player)%2==0 && (game.points[player]>=15 || game.points[1-player]>=15)){
    if(game.points[player]==game.points[1-player]){
      int cp,cNp;

      cp = CountCards(player,game);
      cNp= CountCards(1-player,game);

      if(cp==cNp){
        return 0;
      }
      if(cp>cNp){
        return INFI;
      }
      return -INFI;
    }
    if(game.points[player]>game.points[1-player]){
      return INFI;
    }
    return -INFI;
  }

  ///Luam Jetoane

    ///Luam 1 Jetoan
    if(game.nrGems[player]==9){
      for(x=0;x<GEM_CNT;x++){
        if(game.masa_gems[x]>=1){
          AddJewel(player,x,game);

          aux = Minimax(1-player,adan+1,game,-inm);

          mi=maxim(mi*inm,aux*inm)*inm;

          if(adan==0){
            UpdateAns(aux,1,x,-1,-1);
          }

          RmJewel(player,x,game);
        }
      }
    }

    ///Luam 2 Jetoane
    if(game.nrGems[player]==8){
      for(x=0;x<GEM_CNT;x++){
        for(y=x+1;y<GEM_CNT;y++){
          if(game.masa_gems[x]>=1 && game.masa_gems[y]>=1){
            AddJewel(player,x,game);
            AddJewel(player,y,game);

            aux = Minimax(1-player,adan+1,game,-inm);

            mi=maxim(mi*inm,aux*inm)*inm;

            if(adan==0){
              UpdateAns(aux,1,x,y,-1);
            }

            RmJewel(player,x,game);
            RmJewel(player,y,game);
          }
        }
      }
    }

    ///Luam 3 Jetoane
    if(game.nrGems[player]<=7){
      for(x=0;x<GEM_CNT;x++){
        for(y=x+1;y<GEM_CNT;y++){
          for(z=y+1;z<GEM_CNT;z++){
            if(game.masa_gems[x]>=1 && game.masa_gems[y]>=1 && game.masa_gems[z]>=1){
              AddJewel(player,x,game);
              AddJewel(player,y,game);
              AddJewel(player,z,game);

              aux = Minimax(1-player,adan+1,game,-inm);

              mi=maxim(mi*inm,aux*inm)*inm;

              if(adan==0){
                UpdateAns(aux,1,x,y,z);
              }

              RmJewel(player,x,game);
              RmJewel(player,y,game);
              RmJewel(player,z,game);
            }
          }
        }
      }
    }

  ///Luam 2 Jetoane de aceeasi culoare
  if(game.nrGems[player]<=8){
    for(x=0;x<GEM_CNT;x++){
      if(game.masa_gems[x]>=4){
        AddJewel(player,x,game);
        AddJewel(player,x,game);

        aux = Minimax(1-player,adan+1,game,-inm);

        mi=maxim(mi*inm,aux*inm)*inm;

        if(adan==0){
          UpdateAns(aux,2,x,-1,-1);
        }

        RmJewel(player,x,game);
        RmJewel(player,x,game);
      }
    }
  }


  ///Rezerv
  if(game.nrRez[player]<=2 && (game.nrGems[player]<=9 && game.masa_gems[GOLD]>=1)){
    for(x=1;x<=CARDS_CNT;x++){
      if(game.masa_cards[x]==1){
        game.nrRez[player]++;
        game.rez[player][x]=1;

        int addGold=0;
        if(game.masa_gems[GOLD]>1){
          addGold=1;

          AddJewel(player,GOLD,game);
        }

        aux = Minimax(1-player,adan+1,game,-inm);

        mi=maxim(mi*inm,aux*inm)*inm;

        if(adan==0){
          UpdateAns(aux,3,x,-1,-1);
        }

        if(addGold==1){
          RmJewel(player,GOLD,game);
        }
        game.nrRez[player]--;
        game.rez[player][x]=0;
      }
    }
  }


  ///Cumpar
  for(x=1;x<=CARDS_CNT;x++){
    if(game.masa_cards[x]==1 || game.rez[player][x]==1){
      if(PosibilBuy(player,x,game)){
        int careDintre;

        if(game.masa_cards[x]==1){
          careDintre=0;
          game.masa_cards[x]=0;
        }else{
          careDintre=1;
          game.rez[player][x]=0;
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

        aux = Minimax(1-player,adan+1,game,-inm);

        mi = maxim(mi*inm,aux*inm)*inm;

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
          game.masa_cards[x]=1;
        }else{
          game.rez[player][x]=1;
        }
      }
    }
  }

  return mi;
}


#endif // MINIMAX_H_INCLUDED
