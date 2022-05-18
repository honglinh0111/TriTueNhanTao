/*
	Ho va ten: Huynh Quang Hung
	MSSV: B1809351
	Nhom: 01
*/
#include<stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

using namespace std;
const int BOARD_COL = 3;
const int BOARD_ROW = 3;
const int EMPTY_TILE = 0;
const int MAX_OP = 4;

const char* action[] = {"Trang thai bat dau", 
						"LEN",
						"XUONG",
						"TRAI",
						"PHAI"};
struct State{
	int num[BOARD_ROW][BOARD_COL];
	int empty_row;
	int empty_col;
};

typedef struct Node{
	State state;
	Node *parent;
	int no_function;
	int g;		
	int h;	
	int f;
};
struct node_cmp {
    bool operator() (Node* n1, Node* n2) {
        return n1->f > n2->f;
    }
};
bool sameState (State s1, State s2){
	if (s1.empty_col != s2.empty_col || s1.empty_row != s2.empty_row)
		return false;
	
	for (int row = 0 ; row < BOARD_ROW ; row++)
		for (int col = 0 ; col < BOARD_COL ; col++){
			if (s1.num[row][col] != s2.num[row][col])
				return false;
		}
	
	return true;
}

bool compareStates(State s1, State s2){
	// Kiem tra o trong cua 2 trang thai
	if(s1.empty_row != s2.empty_row || s1.empty_col != s2.empty_col)
		return false;
	// Kiem tra tung vi tri cua 2 trang thai s1 va s2
	for(int row = 0; row < BOARD_ROW; row++) {
		for(int col = 0; col < BOARD_COL; col++) {
			if(s1.num[row][col] != s2.num[row][col]) {
				return false;	
			}	
		}
	}
	return true;
}
bool up (State s, State &out){
	int er = s.empty_row, ec =  s.empty_col;
	if (er > 0){
		out = s;
		out.empty_row = er - 1;
		out.empty_col = ec;
		
		out.num[er][ec] = s.num[er-1][ec];
		out.num[er-1][ec] = EMPTY_TILE;
		
		return true;
	}
	return false;
}


bool down (State s, State &out){
	int er = s.empty_row, ec =  s.empty_col;
	if (er > 0){
		out = s;
		out.empty_row = er + 1;
		out.empty_col = ec;
		
		out.num[er][ec] = s.num[er+1][ec];
		out.num[er+1][ec] = EMPTY_TILE;
		
		return true;
	}
	return false;
}


bool left (State s, State &out){
	int er = s.empty_row, ec =  s.empty_col;
	if (ec > 0){
		out = s;
		out.empty_row = er;
		out.empty_col = ec - 1;
		
		out.num[er][ec] = s.num[er][ec-1];
		out.num[er][ec-1] = EMPTY_TILE;
		
		return true;
	}
	return false;
}


bool right (State s, State &out){
	int er = s.empty_row, ec =  s.empty_col;
	if (ec > 0){
		out = s;
		out.empty_row = er;
		out.empty_col = ec + 1;
		
		out.num[er][ec] = s.num[er][ec+1];
		out.num[er][ec+1] = EMPTY_TILE;
		
		return true;
	}
	return false;
}

bool call_operator(State s, State &out, int op_no){
	switch(op_no){
		case 1:
			return up(s,out);
		case 2:
			return down(s,out);
		case 3:
			return left(s,out);
		case 4:
			return right(s,out);
		default:
			return false;
	}
}

void print_state(State s){
	for (int i = 0 ; i < BOARD_ROW ; i++){
		for (int j = 0 ; j < BOARD_COL ; j ++)
			if(s.num[i][j] == 0)
				printf("_   ");
			else
				printf("%d   ", s.num[i][j]);
				
		printf("\n");
	}
		
}

State *getState(){
	State  *s = new State();
	for (int row = 0 ; row < BOARD_ROW ; row++)
		for (int col = 0 ; col < BOARD_COL ; col++){
			scanf("%d",&s->num[row][col]);
			if (s->num[row][col] == 0){
				s->empty_row=row;
				s->empty_col=col;
			}
		}
	return s;
}

bool is_goal(State s, State goal){
	return sameState(s,goal);
}

int find_state_in_queue(State state, queue<Node*> q) {
	while(!q.empty()) {
		if(compareStates(q.front()->state, state))
			return 1;
		q.pop();
	}
	return 0;
}

int find_state_in_stack(State state, stack<Node*> q) {
	while(!q.empty()) {
		if(compareStates(q.top()->state, state))
			return 1;
		q.pop();
	}
	return 0;
}

void print_WaysToGetGoal(Node* node) {
	stack<Node*> stackPrint;
	
	while(node->parent != NULL) {
		stackPrint.push(node);
		node = node->parent;
	}
	stackPrint.push(node);

	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s\n", no_action, action[stackPrint.top()->no_function]);
		print_state(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}

// Tim kiem chieu rong
Node* BFS(State init_state, State goal_state){

	queue<Node*> Open_BFS;
	queue<Node*> Close_BFS;
	int step = 0;
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = init_state;
	root->parent = NULL;
	root->no_function = 0;
	Open_BFS.push(root);
	while(!Open_BFS.empty()){
		
		Node* node = Open_BFS.front();
		Open_BFS.pop();
		Close_BFS.push(node);
		
		if(compareStates(node->state, goal_state))
			return node;
		
		
		int option;
		for(option=1; option<=MAX_OP; option++) {
			State newstate;
			
			if(call_operator(node->state, newstate, option)) {
				if(find_state_in_queue(newstate, Open_BFS) || find_state_in_queue(newstate, Close_BFS))
					continue;
					
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->parent = node;
				newNode->no_function = option;
				Open_BFS.push(newNode);
			
			}
		}
	}
	return NULL;
}
// Tim kiem chieu sau
Node* DFS(State init_state, State goal_state){
	stack<Node*> Open_DFS;
	stack<Node*> Close_DFS;
	
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = init_state;
	root->parent = NULL;
	root->no_function = 0;
	Open_DFS.push(root);
	while(!Open_DFS.empty()){
		
		Node* node = Open_DFS.top();
		Open_DFS.pop();
		Close_DFS.push(node);
		
		if(compareStates(node->state, goal_state))
		{
			return node;
		}
			
		int option;
		for(option=1; option<=MAX_OP; option++) {
			State newstate;
			if(call_operator(node->state, newstate, option)) {
				
				if(find_state_in_stack(newstate, Open_DFS) || find_state_in_stack(newstate, Close_DFS))
					continue;
					
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->parent = node;
				newNode->no_function = option;
				Open_DFS.push(newNode);
			}
		}
	}
	return NULL;
}

int main(){
	freopen("text.txt","r",stdin);
	
	printf("Trang thai bat dau: \n");
	State *start = getState();
	print_state(*start);
	
	printf("Trang thai muc tieu: \n");
	State *goal = getState();
	printf("\n");
	print_state(*goal);
	
	printf("\n\t------\nB--F--S\n");
	Node* bfs_result = BFS(*start, *goal);
	print_WaysToGetGoal(bfs_result);
	
	printf("\n\t------\nD--F--S\n");
	Node* dfs_result = DFS(*start, *goal);
	printf("\n");
	print_WaysToGetGoal(dfs_result);
	printf("KET THUC");
		
}
