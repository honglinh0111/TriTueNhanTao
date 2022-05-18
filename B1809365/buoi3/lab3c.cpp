//Ho Ten: Pham Thi Hong Linh
//MSSV : B1809365
//STT : 45

//cau c: Giai thuat A*

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
						
const int COLS = 5;
const int ROWS = 5;

//Truong hop me cung 5 dong va 5 cot
const int maze[][COLS] = { {1, 1, 0, 1, 1},
						 { 0, 1, 0, 1, 0 },
						 { 1, 1, 1, 1, 0 },
						 { 0, 1, 0, 1, 0 },
						 { 1, 0, 1, 1, 1 }};

//Trang thai cua me cung					
struct State{
	int robot_x;	//Toa do x cua con robot
	int robot_y;	//Toa do y cua con robot
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
	if (x > COLS || y > ROWS)
		return false;
	if(maze[x][y]==0)
		return false; //ko di chuyen duoc
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

struct node_cmp{
	bool operator()(Node* a, Node* b){
		return a->f < b->f;
	}
};

//h1: dem so vi tri sai khac cua toa do ban dau so voi goal
int h1(State s1, State s2) {
	int count = 0;
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			if(s1.robot_x!=row && s1.robot_y!=col) {
				if(s1.robot_x!=s2.robot_x && s1.robot_y!=s2.robot_y) {
					count++;
				}
			}
		}
	}
	return (count);
}
int heuristic(State s1, State s2, int func) {
	if(func == 1)
		return h1(s1, s2);
	else
		return h1(s1, s2);
}

//kiem tra phan tu co trong frontier
Node* find_node(State state, priority_queue<Node*, vector<Node*>, node_cmp> q) {
	while(!q.empty()) {
		if(same_State(q.top()->state, state))
			return q.top();
		q.pop();
	}  
    return NULL;
}

int find_state_in_priority_queue(State state, priority_queue<Node*, vector<Node*>, node_cmp> q) {
	while(!q.empty()) {
		if(same_State(q.top()->state, state))
			return 1;
		q.pop();
	}
	return 0;
}

Node* Astart(State init_state, State goal_state, int func){
		
	// Khai bao hai hang doi Open va Close
	// Khac voi BFS, trong giai thuat Greedy cung nhu Best First Search,
	// chung ta su dung Hang Doi Uu Tien (xem giao trinh trang 59) 
	// (sap xep thu tu dua tren gia tri f cua cac node)
	priority_queue<Node*, vector<Node*>, node_cmp> priority_queue_Open; 
	priority_queue<Node*, vector<Node*>, node_cmp> priority_queue_Close;
	int step = 0;
	
	// Tao nut trang thai cha
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = init_state;
	root->Parent = NULL;
	root->no_function = 0;
	root->h = heuristic(root->state, goal_state, func);
	root->g = 0;
	root->f = root->h + root->g;
	priority_queue_Open.push(root);
	while(!priority_queue_Open.empty()){
		
		Node* node = priority_queue_Open.top();	// Lay ra gia tri BEN TRAI cua Open de kiem tra
		priority_queue_Open.pop();
		priority_queue_Close.push(node);
		
		// Kiem  tra xem dinh lay ra co phai la trang thai muc tieu
		if(same_State(node->state, goal_state))
		{
			printf("\nTong so nut trong Open la: %d", priority_queue_Open.size());
			printf("\nTong so nut trong Close la: %d", priority_queue_Close.size());
			return node;
		}
		
		
		
		// Goi cac phep toan tren trang thai
		//printf("\n");
		int option;
		for(option=1; option<=4; option++) {
			State newstate;
			
			if(call_operator(node->state, &newstate, option)) {
				
				// Neu trang thai moi sinh ra da ton tai thi bo qua
				
				if(find_state_in_priority_queue(newstate, priority_queue_Close))
					continue;
					
				Node* n= find_node(newstate, priority_queue_Open);
				
				if(n==NULL)	{
					// Neu trang thai moi chua ton tai thi them vao hang doi
					Node *newNode = (Node*)malloc(sizeof(Node));
					newNode->state = newstate;
					newNode->Parent = node;
					newNode->no_function = option;
					newNode->h = heuristic(newstate, goal_state, func);
					newNode->g = node->g+1;
					newNode->f = newNode->h + newNode->g;
					priority_queue_Open.push(newNode);
					
				} else {
					n->g = node->g+1;
                    n->f = n->g+n->h;
				}
			}
		}
	}
	return NULL;
}

void print_WaysToGetGoal_with_Heuristic(Node* node) {
	int no_action = 0;
	stack<Node*> stackPrint;
	
	// Duyet nguoc ve nut parent
	while(node->Parent != NULL) {
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	
	// In ra thu tu hanh dong
	
	while(!stackPrint.empty()){
		printf("\nAction %d: %s\n", no_action, action[stackPrint.top()->no_function]);
		print_state(&stackPrint.top()->state);
		printf("g = %d", stackPrint.top()->g);
		printf("\nh = %d", stackPrint.top()->h);
		printf("\nf = %d", stackPrint.top()->f);
		stackPrint.pop();
		no_action++;
	}
}


int main(){
	State start_state={0,0};
	State newstate;
	printf("\nCau C: Giai thuat A*");
	State goal_state={0,4};
	Node* astart_h1_result = Astart(start_state, goal_state, 1);
	printf("\n");
	print_WaysToGetGoal_with_Heuristic(astart_h1_result);
	return 0;
}



