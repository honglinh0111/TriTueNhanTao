//Ho Ten: Pham Thi Hong Linh
//MSSV : B1809365
//STT : 45

//cau b: Giai thuat BFS => Thu tu duyet cac trang thai
					//=> Tong so trang thai da duyet
					//=> Chuoi cac hang dong can thuc hien => goal

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<set>
#include<queue>
#include<stack>
#include <fstream>
#include <deque>
#include<vector>
#include <functional>
#include<cmath>
#include<malloc.h>
#define goal 4

using namespace std;

//chuoi cac hang dong
const char* action[] = {"Trang thai bat dau",
						"UP",
						"DOWN",
						"LEFT",
						"RIGHT"};

const int COLS = 10;
const int ROWS = 9;

//Truong hop me cung 5 dong va 5 cot
const int maze[][COLS] = { {1, 1, 0, 1, 1},
						 { 0, 1, 0, 1, 0 },
						 { 1, 1, 1, 1, 0 },
						 { 0, 1, 0, 1, 0 },
						 { 1, 0, 1, 1, 1 }};
						 
//Trang thai cua me cung
struct State{
	int robot_x;  //Toa do x cua con robot
	int robot_y;  //Toa do y cua con robot
};

//toa do ban dau cua con robot
void makenull_State(State *state){
	state->robot_x = 0;
	state->robot_y = 0;
}
//In trang thai
void print_state(State *s) {
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			if(s->robot_x==row && s->robot_y==col)
				printf("_ ");
			else
				printf("%d ", maze[row][col]);	
		}
		printf("\n");
	}	
}
int goalCheck(State state){
	return ((state.robot_x==4 && state.robot_y==4)||(state.robot_x==0 && state.robot_y==4)||(state.robot_x==1 && state.robot_y==4));
}
//Dinh nghia kiem tra toa do x , y co hop le khong
bool check_position(int x, int y){
	if(x < 0 || y < 0)
		return false;
	if(x >= ROWS || y>= COLS)
		return false;
	if(maze[x][y] == 0)
		return false;  //ko di chuyen duoc
	return true;
}

//Thu tu len -- xuong -- trai -- phai
//Di chuyen len
bool up(State current_sate, State *new_sate){
	
	int x = current_sate.robot_x-1;
	int y = current_sate.robot_y;
	
	if(check_position(x,y)){
		new_sate->robot_x = x;
		new_sate->robot_y = y;
		return true;
	}else{
		return false;
	}
}

//Di chuyen xuong
bool down(State current_sate, State *new_sate){
	
	int x = current_sate.robot_x+1;
	int y = current_sate.robot_y;
	
	if(check_position(x,y)){
		new_sate->robot_x = x;
		new_sate->robot_y = y;
		return true;
	}else{
		return false;
	}
}

//Di chuyen sang trai
bool left(State current_sate, State *new_sate){
	
	int x = current_sate.robot_x;
	int y = current_sate.robot_y-1;
	
	if(check_position(x,y)){
		new_sate->robot_x = x;
		new_sate->robot_y = y;
		return true;
	}else{
		return false;
	}
}

//Di chuyen sang phai
bool right(State current_sate, State *new_sate){
	
	int x = current_sate.robot_x;
	int y = current_sate.robot_y+1;
	
	if(check_position(x,y)){
		new_sate->robot_x = x;
		new_sate->robot_y = y;
		return true;
	}else{
		return false;
	}
}

//Goi phep toan theo thu tu di chuyen len xuong trai phai
bool call_operator(State s, State *out, int option){
	switch(option){
		case 1:
			return up(s, out);
		case 2:
			return down(s, out);
		case 3:
			return left(s, out);
		case 4:
			return right(s, out);
		default:
			return false;	
	}
}
typedef struct Node{
	State state;
	struct Node *Parent;
	int no_function;
	int g;
	int h;
	int f;
};


//Kiem tra 2 trang thai co giong nhau khong
bool same_State(State s1, State s2){
	if(s1.robot_x != s2.robot_x || s1.robot_y != s2.robot_y){
		return false;
	}
	return true;
}

int find_state_in_queue(State state, queue<Node*> q) {
	while(!q.empty()) {
		if(same_State(q.front()->state, state)){
			return true;
		}			
		q.pop();
	}
	return false;
}

Node* BFS(State state){
		
	// Khai bao hai hang doi Open va Close
	queue<Node*> Open_BFS;
	queue<Node*> Close_BFS;
	// Tao nut trang thai cha
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	Open_BFS.push(root);
	while(!Open_BFS.empty()){
		
		Node* node = Open_BFS.front();	// Lay ra gia tri BEN TRAI cua Open de kiem tra
		Open_BFS.pop();
		Close_BFS.push(node);
		
		// Kiem  tra xem dinh lay ra co phai la trang thai muc tieu
		if(goalCheck(node->state))
		{	return node;
		}
		
		int option;
		for(option=1; option<=4; option++) {
			State newstate;
			makenull_State(&newstate);
			if(call_operator(node->state, &newstate, option)) {
				if(find_state_in_queue(newstate, Open_BFS) || find_state_in_queue(newstate, Close_BFS))
					continue;
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = option;
				Open_BFS.push(newNode);
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal(Node* node) {
	stack<Node*> stackPrint;
	
	// Duyet nguoc ve nut parent
	while(node->Parent != NULL) {
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	
	// In ra thu tu hanh dong
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s\n", no_action, action[stackPrint.top()->no_function]);
		print_state(&stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}

int main(){
	State cur_state = {0, 0}, newstate;
	
	Node *kq = BFS(cur_state);
	print_WaysToGetGoal(kq);
	
	return 0;
}
