//Ho Ten: Pham Thi Hong Linh
//MSSV: B1809365
//STT:45

#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

using namespace std;

const int BOARD_ROWS = 3;
const int BOARD_COLS = 3;
const int EMPTY_TITLE = 0;
const int MAX_OP = 4;

// Hang chuoi de in ra ten cac hanh dong
const char* action[] = {"Trang thai bat dau", 
						"TRAI",
						"PHAI",
						"LEN",
						"XUONG"};

//Dinh nghia trang thai cua bai toan						
struct State {
	int num[BOARD_ROWS][BOARD_COLS];
	int empty_row;		// Luu chi muc dong cua o trong
	int empty_col;		// Luu chi muc cot cua o trong
};

void print_state(State s) {
	for(int row = 0; row < BOARD_ROWS; row++) {
		for(int col = 0; col < BOARD_COLS; col++) {
			if(s.num[row][col] == 0)
				printf("_   ");
			else
				printf("%d   ", s.num[row][col]);	
		}
		printf("\n");
	}	
}

/**
 * Doc du lieu tu file
 */
State* read_file(string file) {
	//ifstream fin(file);
	ifstream fin(file.c_str());
	
	State *s = new State();
	for(int row = 0; row < BOARD_ROWS; row++) {
		for(int col = 0; col < BOARD_COLS; col++) {
			fin >> s->num[row][col];	
		}
	}
	fin >> s->empty_row >> s->empty_col;
	return s;
}


typedef struct Node{
	State state;
	struct Node* parent;
	int no_function;
	int g; //Luu gia tri duong di cua nut goc den nut hien tai
	int h; //Luu ket qua uoc luong cua ham heuristic cua nut hien tai
	int f; //= g+h
}Node;

struct node_cmp{
	bool operator()(Node* a, Node* b){
		return a->f < b->f;
	}
};

//Kiem tra 2 trang thai co giong nhau khong
bool sameState(State s1, State s2){
	if(s1.empty_col != s2.empty_col || s1.empty_row != s2.empty_row){
		return false;
	}
	for (int row=0; row < BOARD_ROWS; row++){
		for(int col=0; col < BOARD_COLS; col++){
			if(s1.num[row][col] != s2.num[row][col]){
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
		out.num[er-1][ec] = EMPTY_TITLE;
		
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

//ham sinh cac phep toan
bool call_operator(State s, State& out, int op_no){
	switch (op_no){
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

//So sanh hai trang thai co bang nhau khong
bool compareStates(State s1, State s2){
	// Kiem tra o trong cua 2 trang thai
	if(s1.empty_row != s2.empty_row || s1.empty_col != s2.empty_col)
		return false;
	
	// Kiem tra tung vi tri cua 2 trang thai s1 va s2
	for(int row = 0; row < BOARD_ROWS; row++) {
		for(int col = 0; col < BOARD_COLS; col++) {
			if(s1.num[row][col] != s2.num[row][col]) {
				return false;	
			}	
		}
	}
	return true;
}

//Kiem tra trang thai co phai la goal
bool is_goal(State s, State goal){
	return sameState(s, goal);
}

//search phan tu da co trong frontier
Node* find_node(State s, multiset <Node *, node_cmp> list){
	for (Node* n: list){
		if (sameState(s, n->state)){
			return n;
		}
	}
	return NULL;
}

// search phan tu da co trong explore
bool find_state(State s, vector <State> *explored){
	for(State c1: *explored){
		if(sameState(s,c1))
			return true;
	}
}

//Nhap trang thai cho bai toan
State* getState(){
	State *s = new State();
	for(int row=0; row < BOARD_ROWS; row++){
		for(int col=0; col<BOARD_COLS; col++){
			cin >> s->num[row][col];
			if(s->num[row][col] == 0){
				s->empty_row = row;
				s->empty_col = col;
			}
		}
	}
	return s;
}

//cac ham can thiet phuc vu cho ham chinh

int find_state_in_queue(State state, queue<Node*> q) {
	while(!q.empty()) {
		if(compareStates(q.front()->state, state))
			return 1;
		q.pop();
	}
	return 0;
}

int find_state_in_stack(State state, stack<Node*> s) {
	while(!s.empty()) {
		if(compareStates(s.top()->state, state))
			return 1;
		s.pop();
	}
	return 0;
}

Node* BFS(State init_state, State goal_state){
		
	// Khai bao hai hang doi Open va Close
	queue<Node*> Open_BFS;
	queue<Node*> Close_BFS;
	int step = 0;
	
	// Tao nut trang thai cha
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = init_state;
	root->parent = NULL;
	root->no_function = 0;
	Open_BFS.push(root);
	while(!Open_BFS.empty()){
		
		//printf("\n");
		//printf("\nOpen BFS: %d", Open_BFS.size());
		//printf("\nClose BFS: %d", Close_BFS.size());
		
		Node* node = Open_BFS.front();	// Lay ra gia tri BEN TRAI cua Open de kiem tra
		Open_BFS.pop();
		Close_BFS.push(node);
		
		// Kiem  tra xem dinh lay ra co phai la trang thai muc tieu
		if(compareStates(node->state, goal_state))
		{
			//printf("\nTong so nut trong Open la: %d", Open_BFS.size());
			//printf("\nTong so nut trong Close la: %d", Close_BFS.size());z			return node;
		}
		
		
		// Goi cac phep toan tren trang thai
		int option;
		for(option=1; option<=MAX_OP; option++) {
			State newstate;
			
			if(call_operator(node->state, newstate, option)) {
				
				// Neu trang thai moi sinh ra da ton tai thi bo qua
				if(find_state_in_queue(newstate, Open_BFS) || find_state_in_queue(newstate, Close_BFS))
					continue;
					
				// Neu trang thai moi chua ton tai thi them vao hang doi
				//printf("\nTrang thai moi duoc dua vao hang doi OPEN\n");
				//print_state(newstate);
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

//3.4.2. Giai thuat duyet theo chieu sau
Node* DFS(State init_state, State goal_state){
	
	// Khai bao hai ngan xep Open va Close
	stack<Node*> Open_DFS;
	stack<Node*> Close_DFS;
	
	// Tao nut trang thai cha
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = init_state;
	root->parent = NULL;
	root->no_function = 0;
	Open_DFS.push(root);
	while(!Open_DFS.empty()){
		
		//printf("\n");
		//printf("\nOpen BFS: %d", Open_DFS.size());
		//printf("\nClose BFS: %d", Close_DFS.size());
		
		// Lay mot dinh trong ngan xep
		Node* node = Open_DFS.top();
		Open_DFS.pop();
		Close_DFS.push(node);
		
		// Kiem  tra xem dinh lay ra co phai la trang thai muc tieu
		if(compareStates(node->state, goal_state))
		{
			//printf("\nTong so nut trong Open la: %d", Open_DFS.size());
			//printf("\nTong so nut trong Close la: %d", Close_DFS.size());
			return node;
		}
			
		int option;
		// Goi cac phep toan tren trang thai
		for(option=1; option<=MAX_OP; option++) {
			State newstate;
			if(call_operator(node->state, newstate, option)) {
				
				// Neu trang thai moi sinh ra da ton tai thi bo qua
				if(find_state_in_stack(newstate, Open_DFS) || find_state_in_stack(newstate, Close_DFS))
					continue;
					
				// Neu trang thai moi chua ton tai thi them vao ngan xep
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

void print_WaysToGetGoal(Node* node) {
	stack<Node*> stackPrint;
	
	// Duyet nguoc ve nut parent
	while(node->parent != NULL) {
		stackPrint.push(node);
		node = node->parent;
	}
	stackPrint.push(node);
	
	// In ra thu tu hanh dong
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nAction %d: %s\n", no_action, action[stackPrint.top()->no_function]);
		print_state(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}

int main() {
	State *start_state;
	
	// Khai bao trang thai bat dau.
	// Trang thai bat dau duoc cau hinh trong file test.txt
	start_state = read_file("start_easy.txt");
	printf("Trang thai bat dau: \n");
	print_state(*start_state);
	
	
	// Khai bao trang thai muc tieu.
	// Trang thai muc tieu duoc cau hinh trong file goal.txt
	State *goal_state;
	goal_state = read_file("goal_easy.txt");
	printf("\n");
	printf("Trang thai muc tieu: \n");
	print_state(*goal_state);
	
	// Tim kiem mu
	printf("\n");
	printf("*** Su dung giai thuat BFS ***");
	Node* bfs_result = BFS(*start_state, *goal_state);
	printf("\n");
	print_WaysToGetGoal(bfs_result);
	printf("\n*** Ket thuc giai thuat BFS ***\n");
	
	printf("\n");
	printf("*** Su dung giai thuat DFS ***");
	Node* dfs_result = DFS(*start_state, *goal_state);
	printf("\n");
	print_WaysToGetGoal(dfs_result);
	printf("\n*** Ket thuc giai thuat DFS ***\n");
	
	return 0;
}

