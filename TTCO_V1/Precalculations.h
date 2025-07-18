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


  for(i=0;i<nr_players;i++){
    for(j=0;j<GEM_CNT;j++){
      game.bonus[i][j] = player_data[i].bonus[j];
    }
  }

  setZero(game.masa_cards);
  setZero(game.notPosibleCards);

  for(i=0;i<RANKS;i++){
    for(j=0;j<MAX_CARDS_ROW;j++){
      if(table_cards[i][j]>=0){
        addBit(game.masa_cards,table_cards[i][j]);
        addBit(game.notPosibleCards,table_cards[i][j]);
      }
    }
  }

  for(i=0;i<GEM_GOLD;i++){
    game.masa_gems[i] = table_gem[i];
  }

  for(i=0;i<nr_players;i++){
    s=0;
    for(j=0;j<GEM_GOLD;j++){
      s+=player_data[i].gems[j];
    }
    game.nrGems[i]=s;
  }

  for(i=0;i<nr_players;i++){
    game.nrRez[i] = player_data[i].nrRez;
  }

  for(i=0;i<nr_players;i++){
    for(j=0;j<GEM_GOLD;j++){
      game.player_gems[i][j]=player_data[i].gems[j];
    }
  }


  for(i=0;i<nr_players;i++){
    game.points[i] = player_data[i].points;
  }

  for(i=0;i<nr_players;i++){
    setZero(game.rez[i]);

    for(j=0;j<game.nrRez[i];j++){
      addBit(game.rez[i],player_data[i].rez[j]);
      addBit(game.notPosibleCards,player_data[i].rez[j]);
    }
  }

  for(i=0;i<nr_players;i++){
    for(j=0;j<player_data[i].nrCards;j++){
      addBit(game.notPosibleCards,player_data[i].cards[j]);
    }
  }

  for(i=0;i<nr_players;i++){
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

DATE CopyGame(const DATE &game){
  int i,j;
  DATE newGame;

  for(j=0;j<2;j++){
    newGame.masa_cards.val[j]=game.masa_cards.val[j];
    newGame.notPosibleCards.val[j]=game.notPosibleCards.val[j];
  }

  for(i=0;i<SNOBNR;i++){
    newGame.masa_snob[i]=game.masa_snob[i];
  }

  for(i=0;i<GEM_GOLD;i++){
    newGame.masa_gems[i]=game.masa_gems[i];
  }

  for(i=0;i<nr_players;i++){
    for(j=0;j<GEM_GOLD;j++){
      newGame.player_gems[i][j]=game.player_gems[i][j];
    }
    for(j=0;j<GEM_CNT;j++){
      newGame.bonus[i][j]=game.bonus[i][j];
    }
    for(j=0;j<=1;j++){
      newGame.rez[i].val[j]=game.rez[i].val[j];
    }

    newGame.nrRez[i]=game.nrRez[i];
    newGame.points[i]=game.points[i];
    newGame.nrGems[i]=game.nrGems[i];
    newGame.nrSnob[i]=game.nrSnob[i];
  }

  newGame.usedRezerve=game.usedRezerve;

  return newGame;
}


#endif // PRECALCULATIONS_H_INCLUDED
