#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#include<stdbool.h>
#include<time.h>


char* tetramino[7];
int field_width=12;
int field_height=18;
unsigned int *pfield;
int screen_width=80;
int screen_height=30;

int Rotate(int px,int py,int r){
    switch (r%4)
    {
    case 0:
        return py*4+px;
    case 1:
        return 12 + py - ( px * 4 );
    case 2:
        return 15 - ( py * 4) - px ;
    case 3:
        return 3 - py + ( px * 4 );
    
    }
}

bool Doesitfit(int tetraminoID,int rotation ,int PosX ,int PosY)
{
    int tIndex;
    int fIndex;
    for(int py=0;py<4;py++){
        for(int px=0;px<4;px++){
            
            tIndex=Rotate(px,py,rotation);
            
            fIndex=((PosY+py)*field_width)+(PosX+px);

            if(PosX+px>=0 && PosX+px<field_width){
                if(PosY+py>=0 && PosY+py<field_height){
                    if(tetramino[tetraminoID][tIndex]=='X'&& pfield[fIndex]!=0){
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

unsigned char getKey(int key){
    switch (key)
    {
    case 0:
        return (unsigned char)'\x27';
    case 1:
        return (unsigned char)'\x25';
    case 2:
        return (unsigned char)'\x28';
    case 3:
        return (unsigned char)'Z';
    }
    return 'E';
}

char getCharacter(int i){
    switch (i)
    {
    case 0 :
        return ' ';
    case 1 :
        return 'A';
    case 2 :
        return 'B';
    case 3 :
        return 'C';
    case 4 :
        return 'D';
    case 5 :
        return 'E';
    case 6 :
        return 'F';
    case 7 :
        return 'G';
    case 8 :
        return '=';
    case 9 :
        return '#';
    default:
        return (char)i;
    }
}

int main(){
    //creating assests
    char* screen;
    screen=(char*)calloc(screen_height*screen_width,sizeof(char));

    tetramino[0]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[0],"..X.");
    strcat(tetramino[0],"..X.");
    strcat(tetramino[0],"..X.");
    strcat(tetramino[0],"..X.");

    tetramino[1]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[1],"....");
    strcat(tetramino[1],".XX.");
    strcat(tetramino[1],".XX.");
    strcat(tetramino[1],"....");

    tetramino[2]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[2],"....");
    strcat(tetramino[2],".X..");
    strcat(tetramino[2],".XX.");
    strcat(tetramino[2],"..X.");

    tetramino[3]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[3],"....");
    strcat(tetramino[3],".XX.");
    strcat(tetramino[3],"..X.");
    strcat(tetramino[3],"..X.");

    tetramino[4]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[4],"..X.");
    strcat(tetramino[4],".XX.");
    strcat(tetramino[4],"..X.");
    strcat(tetramino[4],"....");

    tetramino[5]=(char*)calloc(16,sizeof(char));
    strcat(tetramino[5],"....");
    strcat(tetramino[5],".XX.");
    strcat(tetramino[5],".X..");
    strcat(tetramino[5],".X..");

    pfield=(unsigned int*)malloc(field_height*field_width*sizeof(unsigned int));
    for(int y=0;y<field_height;y++){
        for(int x=0;x<field_width;x++)
        pfield[y*field_width+x]=(x==0 || x==(field_width-1) || y==(field_height-1))?9:0;
    }
    
    for(int i=0;i<screen_height*screen_width;i++)
    screen[i]=' ';
    
    //  for(int y=0;y<field_height;y++){
    //    for(int x=0;x<field_width;x++){
    //         screen[(y+2)*screen_width+(x+2)]=getCharacter(pfield[y*field_width+x]);
    //     }
    // }
    
    HANDLE console_buffer= CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(console_buffer);
    DWORD words_written =0;
    COORD origin;
    origin.X=0;
    origin.Y=0;
    bool isGameOver =false;
   SMALL_RECT size;
    size.Top=0;
    size.Left=0;
    size.Right=80;
    size.Bottom=30;
    //system("MODE 80,30");
    SetConsoleWindowInfo(console_buffer,true,&size);
    int currentID=0;
    int currentx=field_width/2;
    int currenty=0;
    int currentrotation=0;

    bool bkey[4];
    bool isthekeyHeld=false;
    int droprate=1;
    int maxdroprate=10;
    int speed=20;
    int speedCounter=0;
    bool forcedown=false;
    int score=0;
    int linecleared=0;
    int lineclearedP=0;
    srand(time(NULL));
    int clearYpos[]={-1,-1,-1,-1};
    
    while(!isGameOver){ 
       //Game timer=======================================================================
                Sleep(50);
                speedCounter++;
            if(speedCounter >= speed){
                speedCounter=0;
                forcedown=true;
            }
    
       
       //Game input=======================================================================
       for(int i =0;i<4;i++){
           bkey[i]=(0x8000 & GetAsyncKeyState(getKey(i)))!=0;
       }
       if(bkey[1]){
           if(Doesitfit(currentID,currentrotation,currentx-1,currenty)){
               currentx-=1;
           }
       }
       if(bkey[0]){
           if(Doesitfit(currentID,currentrotation,currentx+1,currenty)){
               currentx+=1;
           }
       }
       if(bkey[2]){
           if(Doesitfit(currentID,currentrotation,currentx,currenty+1) ){
               currenty+=1;
           }
       }
       if(bkey[3]){
           if(!isthekeyHeld && Doesitfit(currentID,(currentrotation+1)%4,currentx,currenty)){
               currentrotation=(currentrotation+1)%4;
               isthekeyHeld=true;
           }
       }
       else
           {
               isthekeyHeld=false;
           }
       //Game logic=======================================================================
    
       if(forcedown){
           if(Doesitfit(currentID,currentrotation,currentx,currenty+1)){
               currenty++;
           }
           else{
            
               //lock the piece in place
               for(int py=0;py<4;py++){
                   for(int px=0;px<4;px++){
                       if(tetramino[currentID][Rotate(px,py,currentrotation)]=='X')
                       pfield[(currenty+py)*field_width+(px+currentx)]=getCharacter(currentID+1);
                   }
               }
    
               //check for any complete lines
               for(int py=0;py<4;py++){
                   if(currenty+py < field_height-1){
                       bool line=true;
                       for(int px=1;px<field_width-1;px++){
                           if(pfield[(currenty+py)*field_width+px]==0)
                           line=false;
                       }
                       if(line){
                           clearYpos[py]=currenty+py;
                           for(int px=1;px<field_width-1;px++){
                           pfield[(currenty+py)*field_width+px]=8;
                           }
                       }
                   }
               }
    
               currentID=((rand()*6)/RAND_MAX);
               currentx=field_width/3;
               currenty=0;
               currentrotation=((rand()*4)/RAND_MAX);
           }
            forcedown=false;
       }
    
    
       //draw field
       for(int y=0;y<field_height;y++){
           for(int x=0;x<field_width;x++){
               screen[(y+2)*screen_width+(x+2)]=getCharacter(pfield[y*field_width+x]);
           }
       }
    
       //draw current piece================================================================
       for(int py=0;py<4;py++){
           for(int px=0;px<4;px++){
               if(tetramino[currentID][Rotate(px,py,currentrotation)]=='X'){
                   screen[(currenty+py+2)*screen_width+(currentx+px+2)]=getCharacter(currentID+1);
               }
           }
       }
       //clearing lines
       if(clearYpos[0]!=-1 || clearYpos[2]!=-1 || clearYpos[2]!=-1 || clearYpos[3]!=-1){
            WriteConsoleOutputCharacter(console_buffer,screen,screen_height*screen_width,origin,&words_written);
            Sleep(500);
            for(int i=0;i<4;i++){
                if(clearYpos[i]!=-1){
                    for(int py=clearYpos[i];py>0;py--){
                        for(int px=1;px<field_width-1;px++){
                            pfield[py*field_width+px]=pfield[(py-1)*field_width+px];
                            //pfield[px]=0;
                        }
                    }
                    clearYpos[i]=-1;
                    linecleared++;
                }
            }
            if(linecleared==1)
            score+=40;
            if(linecleared==2)
            score+=100;
            if(linecleared==3)
            score+=300;
            if(linecleared==4)
            score+=800;
            if(linecleared==4&&lineclearedP==4)
            score+=400;
            lineclearedP=linecleared;
            linecleared=0;
        }


       
        //update score board


        sprintf(&screen[1*screen_width+2],"SCORE%8d",score);

    
       // display frame
       WriteConsoleOutputCharacter(console_buffer,screen,screen_height*screen_width,origin,&words_written);
       //Game over condition
       for(int px=1;px<field_width-1;px++){
            if(pfield[field_width+px]!=0){
            isGameOver=true;
            break;
            }
       }
    
       }
       CloseHandle(console_buffer);
       printf("Game over .\nYour Score is : %d\n",score);
    
    
    
    return 0;
    
}      