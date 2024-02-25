#include <iostream>// to include input and output
#include <conio.h>//to include finctions for console input/output like keyboard controller
#include <dos.h> 
#include <windows.h>//to include all of the function in windows API
#include <time.h>//to get and manipulate date and time information

#include<math.h>
#include<string.h>

// constant
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70 

using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);//to access the input and screen buffers of its console.
COORD CursorPosition;// It is a structure to hold screen COORDinates X and Y.


bool isPlaying = true;
int enemyY[3];
int enemyX[3];
int enemyFlag[3];

char car[4][4] = { ' ','&','&',' ', 
					'&','&','&','&', 
					' ','&','&',' ',
					'&','&','&','&' }; 
					

				
int carPos = WIN_WIDTH/2;
int score = 0; 

int highestScore = 0;



void gotoxy(int x, int y){ //If you have declared this function, you can normally use gotoxy(x,y)
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition); //Sets the cursor position in the console screen buffer.
	//set cursor to specified place in console that depends on cord.x and cord.y. 
}

void setcursor(bool visible, DWORD size) {//to hide cursor
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void drawBorder(){ // to creat border 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			gotoxy(0+j,i); cout<<"?";
			gotoxy(WIN_WIDTH-j,i); cout<<"?";
		}
	} 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"?";
	} 
}
void genEnemy(int ind){   // ind means intermediate numbers
	enemyX[ind] = 17 + rand()%(33);  //rand random numbers

}
// to draw enemy in the screen 
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"****";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<" ** "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"****"; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<" ** ";  
		
}
}


void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    "; 
	} 
}


void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}


void drawCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<car[i][j];
		}
	}
}
void eraseCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<" ";
		}
	}
}
 
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9  ){
			return 1;
		}
	}
	return 0;
} 
void gameover(){
	system("cls");
	system ("color D0");
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h,7);
		 
    if (score > highestScore) {   // high score
            highestScore = score;
        }
	cout<<endl;
	cout<<"\t\t-------------------------------"<<endl;
	cout<<"\t\t--------  Game Over   ---------"<<endl;
	cout<<"\t\t--------              ---------"<<endl;
    cout<<"\t\t-------- Score :      "<<score<<"---------"<<endl;
	cout<<"\t\t-------- High Score : "<<highestScore<<"--------"<<endl;
	cout<<"\t\t-------------------------------"<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}
	


void instructions(){
		system("color 70");
	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Avoid Cars by moving left or right. ";
	cout<<"\n\n Press 'a' to move left";
	cout<<"\n Press 'd' to move right";
	cout<<"\n Press 'p' to pause the game";
	cout<<"\n Press 'escape' to exit";

	cout<<"\n\nPress any key to go back to menu";
	getch();
}

void play(){
	system("color B0");
	carPos = -1 + WIN_WIDTH/2;
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	
	
	system("cls"); 
	drawBorder(); 
	updateScore();

	
	genEnemy(0);
	genEnemy(1);
	
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Car Game";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";	
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Control    ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"--------  ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<"  A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15);cout<<"  D Key - Right"; 
	gotoxy(WIN_WIDTH + 2, 16);cout<<"  P Key - Pause";
	
	gotoxy(18, 5);cout<<" Press any key to start";
	getch();
	gotoxy(18, 5);cout<<"                             "; 
	
	while(true){
		if(kbhit()){  // _kbhit() function from the "conio.h" library to detect if a key has been pressed. 
			char ch = getch();
			if( ch =='a' || ch =='A' ){
				if( carPos > 18 )
					carPos -= 4;
			}
			if( ch =='d' || ch =='D' ){
				if( carPos < 50 )
					carPos += 4;
			} 
             
			if(ch == 27){
				break;
			}
			if( ch =='p' || ch =='P' ){
			   gotoxy(18, 5); cout << "The Game has been paused"<< endl;
               gotoxy(19, 7); system("pause");
               	getch(); 
	gotoxy(18, 5);cout<<"                               "; 
	gotoxy(19, 7);cout<<"                               "; 
	
			} 
			
			}
		
		
		
		drawCar(); 
		drawEnemy(0); 
		drawEnemy(1); 
		if( collision() == 1  ){
			gameover();
			return;
		} 
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);   
		
		if( enemyY[0] == 10 )
			if( enemyFlag[1] == 0 )
				enemyFlag[1] = 1;
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;
		 
		if( enemyY[0] > SCREEN_HEIGHT-4 ){
			resetEnemy(0);
			score++;
			updateScore();
		}
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score++;
			updateScore();
		}
		
	
		
}
}
int main(){


	setcursor(0,0); 
	srand( (unsigned)time(NULL));    
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	 
	do{
		
		system("cls");
		system ("color B0");
			SetConsoleTextAttribute(h,7);
		gotoxy(10,5); cout<<" -------------------------- "; 
		gotoxy(10,6); cout<<" |        Car Game        | "; 
		gotoxy(10,7); cout<<" -------------------------- ";
		gotoxy(10,9); cout<<" --------------------------";
		gotoxy(10,10); cout<<"   1.    Start Game        ";
		gotoxy(10,11); cout<<"   2.   Instructions       ";	 
		gotoxy(10,12); cout<<"   3.      Quit            ";
		gotoxy(10,13); cout<<"-------------------------- ";
		gotoxy(10,15); cout<<"   Select option:   ";
		char op = getche();
		
		if( op=='1') play();
		else if( op=='2') instructions();
		else if( op=='3') exit(0);
		
	}while(1);

    return 0;
}









