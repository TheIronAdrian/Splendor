#ifndef READ_H_INCLUDED
#define READ_H_INCLUDED

#define MAX_PLAYERS 4
#define MAX_CARDS_ROW 4
#define MAX_REZ 3
#define RANKS 3
#define MOD 31

struct PLAYER{
  int gems[GEM_GOLD];
  int bon[GEM_CNT];
  int rez[MAX_REZ];
  int nrRez;
  int cards[CARDS_CNT];
  int nrCards;
  int nobl[NOBLE_CNT];
  int nrNobl;
  int bonus[GEM_CNT];
  int points;
  int bitmasks[3];
};

int nr_players,nr_nobl,id,round_nr;

PLAYER player_data[MAX_PLAYERS];
int table_gem[GEM_GOLD];
int table_nobl[NOBLE_CNT];
int table_cards[RANKS][MAX_CARDS_ROW];

void Read_Table(){
  int i,j,usless;

  scanf("%d %d",&nr_players,&id);
  id--;
  scanf("%d",&round_nr);

  for(i=0;i<GEM_GOLD;i++){
    scanf("%d",&table_gem[i]);
  }

  for(i=0;i<RANKS;i++){
    scanf("%d ",&usless);
    for(j=0;j<MAX_CARDS_ROW;j++){
      scanf("%d",&table_cards[i][j]);
    }
  }

  scanf("%d",&nr_nobl);
  for(i=0;i<nr_nobl;i++){
    scanf("%d",&table_nobl[i]);
  }


  for(i=0;i<nr_players;i++){
    for(j=0;j<GEM_GOLD;j++){
      scanf("%d",&player_data[i].gems[j]);
    }

    scanf("%d",&player_data[i].nrCards);
    for(j=0;j<player_data[i].nrCards;j++){
      scanf("%d",&player_data[i].cards[j]);
    }

    scanf("%d",&player_data[i].nrRez);
    for(j=0;j<player_data[i].nrRez;j++){
      scanf("%d",&player_data[i].rez[j]);
    }

    scanf("%d",&player_data[i].nrNobl);
    for(j=0;j<player_data[i].nrNobl;j++){
      scanf("%d",&player_data[i].nobl[j]);
    }
  }
}

#endif // READ_H_INCLUDED
