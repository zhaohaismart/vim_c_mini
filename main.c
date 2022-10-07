#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define VERSION "2.0.6"

//inner functions
int menu(void);
int GetText(char name[]);
int AddChar(char ch);
int DeletChar(void);
int NewLine(void);
int FindWord(char word[]);
int ** mallocText(int size1,int size2);
int ** reallocText(void);
void RenewText(void);
void TextClean(int ** map);
void TextPrint(void);
void StoreText(char name[]);
void CountLines(void);
void AllPullDown(void);
void AllPullUp(void);

//inner variables
int ** text;
int ROWS=2;
int COLS=101;
int x=0,y=0;
int curows=0;
int firows=24;
int cucols=0;
int ficols=79;
int lines=0;
int choice=0;
int ch='\0';
int num=0;
char function;
char number[4];
char word[40];
_Bool control=0;
_Bool end=0;

int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Not enough arguments!\n");
        printf("edit -filename\n");
        return 0;
    }
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1,COLOR_RED,COLOR_CYAN);
    init_pair(2,COLOR_GREEN,COLOR_BLUE);
    text=mallocText(ROWS,COLS);
    TextClean(&text[0]);
    keypad(stdscr,TRUE);
    choice=menu();
    while(choice!=3){
        if(choice==0){
            GetText(argv[1]);
            CountLines();
            TextPrint();
            refresh();
            ch=getch();
            clear();
            while(!end){
                CountLines();
                switch(ch){
                    case KEY_UP:
                        if((x-1>=0)&&(text[x-1][y]!='\0'))
                            x--;
                        else
                            beep();
                        control=1;
                        break;
                    case KEY_DOWN:
                        if((x+1<ROWS)&&(text[x+1][y]!='\0'))
                            x++;
                        else
                            beep();
                        control=1;
                        break;
                    case KEY_LEFT:
                        if((y-1>=0)&&(text[x][y-1]!='\0'))
                            y--;
                        else
                            beep();
                        control=1;
                        break;
                    case KEY_RIGHT:
                        if((y+1<COLS)&&(text[x][y]!='\0')&&(text[x][y]!='\n'))
                            y++;
                        else
                            beep();
                        control=1;
                        break;
                    case 127: //backspace, I don't know why KEY_BACKSPACE doesn't work.
                        if(y-1>=0){
                            y--;
                            DeletChar();
                            control=1;
                            break;
                        }
                        if((lines-1>=0)&&(y-1<0)){
                            AllPullUp();
                            lines--;
                            y=0;
                            if(x-1>=0)
                                x--;
                            control=1;
                            break;
                        }
                        control=1;
                        break;
                    case 27:
                        end=1;
                        control=1;
                        break;
                    default:
                        control=0;
                        break;
                }
                if((!control)&&(ch!='\n')){
                    if(y+2<COLS){
                        AddChar(ch);
                        y++;
                    }
                    else{
                        x++;
                        y=0;
                        beep();
                    }
                }
                if(ch=='\n'){
                    RenewText();
                    if(lines+1<ROWS){
                        NewLine();
                    }
                    else
                        beep();
                }
                TextPrint();
                mvprintw(21,60,"file:%s\n",argv[1]);
                mvprintw(22,60,"Position:%d %d\n",x,y);
                refresh();
                if(!end)
                    ch=getch();
                clear();
            }
            text[lines][0]=-10;
            StoreText(argv[1]);
            end=0;
        }
        if(choice==1){
            clear();
            GetText(argv[1]);
            CountLines();
            TextPrint();
            function=getch();
            while(function!=27){
                if(function=='j'){
                    mvprintw(23,0,"                                                   ");
                    mvprintw(23,0,"j");
                    echo();
                    getstr(number);
                    noecho();
                    clear();
                    if((number[1]!='s')&&(number[1]!='e')){
                        num=atoi(number);
                        if(num>lines)
                            beep();
                        else{
                            x=num;
                            y=1;
                        }
                    }
                    if(number[1]=='s'){
                        x=0;
                        y=1;
                    }
                    if(number[1]=='e'){
                        x=lines-2;
                        y=1;
                    }
                    TextPrint();
                }
                if(function=='f'){
                    mvprintw(23,0,"                                                   ");
                    mvprintw(23,0,"f");
                    echo();
                    getstr(word);
                    noecho();
                    clear();
                    FindWord(word);
                    TextPrint();
                }
                if(function=='d'){
                    mvprintw(23,0,"                                                   ");
                    mvprintw(23,0,"d");
                    echo();
                    getstr(number);
                    noecho();
                    clear();
                    num=atoi(number);
                    if((text[x+num][y]=='\0')||(x+num>ROWS)){
                        num=0;
                        beep();
                    }
                    x=x+num;
                    TextPrint();
                }
                if(function=='u'){
                    mvprintw(23,0,"                                                   ");
                    mvprintw(23,0,"u");
                    echo();
                    getstr(number);
                    noecho();
                    clear();
                    num=atoi(number);
                    if((x-num<0)||(text[x-num][y]=='\0')){
                        num=0;
                        beep();
                    }
                    x=x-num;
                    TextPrint();
                }
                function=getch();
            }
        }
        if(choice==2){
            clear();
            mvprintw(8,15,"BUBU TextEditor %s\n",VERSION);
            mvprintw(9,15,"Written by GooglChen on 2022/4/17\n");
            mvprintw(10,15,"Location: Shanghai, China\n");
            mvprintw(11,15,"To thank and memorize cunzhi school, also the classmates.\n");
            mvprintw(12,15,"Any problems, plz check Readme.txt");
            refresh();
            getch();
            clear();
        }
        choice=menu();
    }
    free(text);
    endwin();
    return 0;
}

int menu(void){  /*This function show the menu to customers*/
    int selection=0;
    int choice=0;
    int i=0,j=0;
    static char welcom[4][20]={"Edit the File\n","More functions\n","About the Arthor\n","Quit\n"};
    clear();
    while(selection!='\n'){
        for(i=0;i<4;i++){
            if(i==choice){
                attron(COLOR_PAIR(2));
                for(j=0;j<20;j++){
                    mvprintw(10+i,32+j,"%c",welcom[i][j]);
                }
                attroff(COLOR_PAIR(2));
            }
            else
                for(j=0;j<20;j++){
                    mvprintw(10+i,32+j,"%c",welcom[i][j]);
                }
        }
        selection=getch();
        switch(selection){
            case KEY_UP:
                if(choice-1>=0)
                    choice--;
                else
                    beep();
                break;
            case KEY_DOWN:
                if(choice+1<=3)
                    choice++;
                else
                    beep();
                break;
            default:
                beep();
                break;
        }
    }
    clear();
    return choice;
}

void TextClean(int ** map){ /*This function inits the whole text.*/
    int i=0,j=0;
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++)
            map[i][j]='\0';
    }
}

int ** mallocText(int size1,int size2){
    int i=0;
    int **res;
    res=(int **)malloc(size1*sizeof(int *));
    for(i=0;i<size1;i++){
        *(res+i)=malloc(size2*sizeof(int));
    }
    return res;
}

int ** reallocText(void){
    int ** res;
    int i=0;
    int j=0;
    ROWS++;
    res=mallocText(ROWS,COLS);
    TextClean(&res[0]);
    for(i=0;i<ROWS-1;i++){
        for(j=0;j<COLS;j++)
            res[i][j]=text[i][j];
    }
    return res;
}

void RenewText(void){
    text=reallocText();
}

void TextPrint(void){ /*This function print the text on the screen.*/
    int i=0,j=0;
    if(x>=24){
        curows=x-23;
        firows=x+1;
    }
    else if(x<24){
        curows=0;
        firows=24;
    }
    if(y>=80){
        cucols=y-79;
        ficols=y+1;
    }
    else if(y<80){
        cucols=0;
        ficols=79;
    }
    for(i=curows;i<firows&&i<ROWS;i++){
        for(j=cucols;j<ficols&&j<COLS;j++){
            if((i==x)&&(j==y)){
                attron(COLOR_PAIR(1));
                if(text[i][j]=='\0')
                    printw(" ");
                if(text[i][j]=='\n'){
                    printw(" ");
                    printw("\n");
                }
                if((text[i][j]!='\0')&&(text[i][j]!='\n'))
                    printw("%c",text[i][j]);
                attroff(COLOR_PAIR(1));
            }
            else
                printw("%c",text[i][j]);
        }
    }
}

void StoreText(char name[]){ /*This function stores the text into the file.*/
    FILE *fp=NULL;
    int i=0,j=0;
    _Bool end=0;
    remove(name);
    fp=fopen(name,"a+");
    if(fp==NULL){
        printw("Unkown errors happened, please check your computer!\n");
        refresh();
    }
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            if(text[i][j]==-10){
                end=1;
                break;
            }
            if((text[i][j]!='\0')&&(text[i][j]!=-10))
                fprintf(fp,"%c",text[i][j]);
        }
        if(end)
            break;
    }
    fclose(fp);
}

int GetText(char name[]){ /*This function gets the text from the file.*/
    FILE *fp=NULL;
    int i=0,j=0;
    char c='\0';
    fp=fopen(name,"r");
    if(fp==NULL){
        printw("Didn't find the file, create a new one.\n");
        refresh();
        getch();
        clear();
        return 1;
    }
    while(!feof(fp)){
        fscanf(fp,"%c",&c);
        if(c!='\n'){
            text[i][j]=c;
            j++;
        }
        else{
            RenewText();
            text[i][j]='\n';
            j=0;
            i++;
        }
    }
    fclose(fp);
    return 0;
}

void CountLines(void){
    int i=0;
    int j=0;
    lines=0;
    for(i=0;i<ROWS;i++){
        if(text[i][0]!='\0')
            lines++;
        else
            break;
    }
}

int AddChar(char ch){ /*This function add a new character int to the text.*/
    int m=0,n=0;
    char buf[COLS+1];
    _Bool overflow=0;
    for(m=0;m<COLS+1;m++)
        buf[m]='\0';
    m=0;
    while((text[x][y+n]!='\0')&&(y+n<COLS-1)){
        buf[m]=text[x][y+n];
        m++;
        n++;
    }
    if(y+n>=COLS-1)
        overflow=1;
    text[x][y]=ch;
    m=0;
    n=1;
    while(buf[m]!='\0'){
        text[x][y+n]=buf[m];
        m++;
        n++;
    }
    if(overflow)
        return 1;
    else
        return 0;
}

int DeletChar(void){ /*This function delet a char.*/
    int n=0;
    char buf[COLS+1];
    _Bool overflow=0;
    for(n=0;n<COLS+1;n++)
        buf[n]='\0';
    n=0;
    while((text[x][y+n+1]!='\0')&&(y+n+1<COLS-1)){
        buf[n]=text[x][y+n+1];
        text[x][y+n+1]='\0';
        n++;
    }
    if(y+n+1>=COLS-1)
        overflow=1;
    n=0;
    while(buf[n]!='\0'){
        text[x][y+n]=buf[n];
        n++;
    }
    if(overflow)
        return 1;
    else
        return 0;
}

int NewLine(void){ //This function replys to the event '\n'
    int n=0;
    char buf[COLS+1];
    for(n=0;n<COLS+1;n++)
        buf[n]='\0';
    n=0;
    while((text[x][y+n]!='\0')&&(y+n<COLS-1)){
        buf[n]=text[x][y+n];
        text[x][y+n]='\0';
        n++;
    }
    AllPullDown();
    text[x][y]='\n';
    n=0;
    while(buf[n]!='\0'){
        text[x+1][0+n]=buf[n];
        n++;
    }
    x++;
    y=0;
    return 0;
}

void AllPullDown(void){
    int n=0;
    int l=0;
    for(l=lines;l>x;l--){
        while(text[l][n]!='\0'){
            text[l+1][n]=text[l][n];
            n++;
        }
        n=0;
        while(text[l][n]!='\0'){
            text[l][n]='\0';
            n++;
        }
        n=0;
    }
    n=0;
    l=0;
    while(text[x+1][n]!='\0'){
        text[x+1][n]='\0';
        n++;
    }
}

void AllPullUp(void){
    int n=0;
    int l=0;
    for(l=x;l<lines;l++){
        while(text[l][n]!='\0'){
            text[l][n]='\0';
            n++;
        }
        n=0;
        while(text[l+1][n]!='\0'){
            text[l][n]=text[l+1][n];
            n++;
        }
        n=0;
    }
    n=0;
    while(text[lines][n]!='\0'){
        text[lines][n]='\0';
        n++;
    }
}

int FindWord(char word[]){ /*This function can find words*/
    int length=0;
    int n=0;
    int m=0;
    _Bool find=0;
    int i=0,j=0;
    for(i=x+1;i<lines;i++){
        for(j=0;j<COLS;j++){
            if(text[i][j]==word[1]){
                m=j;
                while(word[n+1]!='\0'){
                    if(text[i][j]==word[n+1])
                        find=1;
                    else{
                        find=0;
                        while((text[i][j]!=' ')&&(text[i][j]!='\0'))
                            j++;
                        n=0;
                        break;
                    }
                    j++;
                    n++;
                }
            }
            if(find)
                break;
        }
        if(find){
            x=i;
            y=m+1;
            break;
        }
        else{
            x=0;
            y=0;
        }
    }
    if(find)
        return 1;
    else
        return 0;
}
