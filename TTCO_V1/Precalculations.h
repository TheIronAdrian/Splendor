#ifndef PRECALCULATIONS_H_INCLUDED
#define PRECALCULATIONS_H_INCLUDED

void CalculatePointsBef(){
  int i,j,s,id_carte;

  for(i=0;i<nr_players;i++){
    s=0;

    for(j=0;j<player_data[i].nrCards;j++){
      id_carte=player_data[i].cards[j];

      s+=CARDS[id_carte][POINTS];
    }

    s+=player_data[i].nrNobl*3;

    player_data[i].points=s;
  }
}

void CalculateBonusBef(){
  int i,j,id_carte;

  for(i=0;i<nr_players;i++){

    for(j=0;j<player_data[i].nrCards;j++){
      id_carte=player_data[i].cards[j];

      player_data[i].bonus[CARDS[id_carte][BONUS]]++;
    }
  }
}

void CalculateMasks(){
  int i,j,x,y,id_carte;

  for(i=0;i<nr_players;i++){
    for(j=0;j<player_data[i].nrRez;j++){
      id_carte = player_data[i].rez[j];
      if(id_carte!=-1){
        player_data[i].bitmasks[id_carte/MOD]|=1<<(id_carte%MOD);
      }
    }
  }

  for(x=0;x<RANKS;x++){
    for(y=0;y<MAX_CARDS_ROW;y++){
      id_carte = table_cards[x][y];

      if(id_carte!=-1){
        for(i=0;i<nr_players;i++){
          player_data[i].bitmasks[id_carte/MOD]|=1<<(id_carte%MOD);
        }
      }
    }
  }
}

DATE MakeBaseGame(){
  DATE game;
  int i,j,s;


  for(i=0;i<2;i++){
    for(j=0;j<GEM_CNT;j++){
      game.bonus[i][j] = player_data[i].bonus[j];
    }
  }

  for(i=0;i<CARDS_CNT;i++){
    game.masa_cards[i]=0;
  }
  for(i=0;i<RANKS;i++){
    for(j=0;j<MAX_CARDS_ROW;j++){
      if(table_cards[i][j]>=0){
        game.masa_cards[table_cards[i][j]]=1;
      }
    }
  }

  for(i=0;i<GEM_GOLD;i++){
    game.masa_gems[i] = table_gem[i];
  }


  for(i=0;i<2;i++){
    s=0;
    for(j=0;j<GEM_GOLD;j++){
      s+=player_data[i].gems[j];
    }
    game.nrGems[i]=s;
  }

  for(i=0;i<2;i++){
    game.nrRez[i] = player_data[i].nrRez;
  }

  for(i=0;i<2;i++){
    for(j=0;j<GEM_GOLD;j++){
      game.player_gems[i][j]=player_data[i].gems[j];
    }
  }


  for(i=0;i<2;i++){
    game.points[i] = player_data[i].points;
  }

  for(i=0;i<2;i++){
    for(j=0;j<=CARDS_CNT;j++){
      game.rez[i][j]=0;
    }

    for(j=0;j<game.nrRez[i];j++){
      game.rez[i][player_data[i].rez[j]]=1;
    }
  }

  for(i=0;i<2;i++){
    game.nrSnob[0]=player_data[i].nrNobl;
  }

  for(i=0;i<nr_nobl;i++){
    game.masa_snob[i]=table_nobl[i];
  }

  for(;i<SNOBNR;i++){
    game.masa_snob[i]=-1;
  }

  game.usedRezerve=0;

  return game;
}


#endif // PRECALCULATIONS_H_INCLUDED
