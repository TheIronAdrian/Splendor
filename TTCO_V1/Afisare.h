#ifndef AFISARE_H_INCLUDED
#define AFISARE_H_INCLUDED

void Afisare(MOVE ans){
  int i,s;

  printf("%d ",ans.type);
  if(ans.type==1){
    s=0;

    for(i=0;i<3;i++){
      if(ans.rasp[i]!=-1){
        s++;
      }
    }

    printf("%d ",s);
    for(i=0;i<3;i++){
      if(ans.rasp[i]!=-1){
        printf("%d ",ans.rasp[i]);
      }
    }

    printf("\n");
    return;
  }

  if(ans.type==2){
    printf("%d \n",ans.rasp[0]);
  }

  if(ans.type==3){
    for(i=0;i<3;i++){
      if(ans.rasp[i]!=-1){
        printf("%d ",ans.rasp[i]);
      }
    }
    printf("\n");
  }

  if(ans.type==4){
    printf("%d\n",ans.rasp[0]);
  }
}

void AfisareMCTS(MOVE ans){
  int i,s;

  printf("%d ",ans.type);
  if(ans.type==1){
    s=ans.rasp[0];
    printf("%d ",s);
    for(i=1;i<=3;i++){
      if(ans.rasp[i]!=-1){
        printf("%d ",ans.rasp[i]);
      }
    }

    printf("\n");
    return;
  }

  if(ans.type==2){
    printf("%d \n",ans.rasp[0]);
  }

  if(ans.type==3){
    for(i=0;i<3;i++){
      if(ans.rasp[i]!=-1){
        printf("%d ",ans.rasp[i]);
      }
    }
    printf("\n");
  }

  if(ans.type==4){
    printf("%d\n",ans.rasp[0]);
  }
}

#endif // AFISARE_H_INCLUDED
