// Ho Ten : Pham Thi Hong Linh
// MSSV : B1809365
// STT : 45

//Bai 3: Bieu dien cay tim kiem theo chieu rong

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

const char* action[] = {"First State", "pour Water Full X", 
						"pour Water Full Y",
						"pour Water Empty X", "pour Water Empty Y",
						"pour Water X to Y", "pour Water Y to X"};

#define max_x 9 //Suc chua binh X
#define max_y 4 //Suc chua binh Y
#define empty1 0
#define goal 6 //Muc tieu luong nuoc can dong duoc
#define maxlength 100 //Su dung cai dat hang doi
//Khai bao cau truc trang thai
typedef struct{
	int x; //luong nuoc trong binh X
	int y; //luong nuoc trong binh Y
}State;
//Khoi tao trang thai binh nuoc x=0, y=0
void makeNullState(State *state){
	state->x = 0;
	state->y = 0;
}
//In trang thai
void print_State(State state){
	printf("\n	X:%d --- Y:%d", state.x, state.y);
}
//Ham kiem tra trang thai muc tieu
int goalcheck(State state){
	return (state.x == goal || state.y == goal);
}
//Lam day nuoc binh X
int pWFullX(State cur_state, State *result){
	if(cur_state.x < max_x){
		result->x = max_x;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
//Lam day nuoc binh Y
int pWFullY(State cur_state, State *result){
	if(cur_state.y < max_y){
		result->x = cur_state.x;
		result->y = max_y;		
		return 1;
	}
	return 0;
}
//Lam rong nuoc trong X
int pWEmptyX(State cur_state, State *result){
	if(cur_state.x > 0){
		result->x = empty1;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
//Lam rong nuoc trong Y
int pWEmptyY(State cur_state, State *result){
	if(cur_state.y > 0){
		result->x = cur_state.x;
		result->y = empty1;
		return 1;
	}
	return 0;
}
//tim max
int max(int x, int y){
	if(x>y)	
		return x;
	else
		return y;
}
//tim min
int min(int x, int y){
	if(y<x)	return y;
	else return x;
}
//Chuyen nuoc tu binh X sang binh Y
int pWXY(State cur_state, State *result){
	if(cur_state.x > 0  &&  cur_state.y < max_y){
		result->x = max(cur_state.x - (max_y - cur_state.y), empty1);
		result->y = min(cur_state.x + cur_state.y, max_y);
		return 1;
	}
	return 0;
}
//Chuyen nuoc tu binh Y sang binh X
int pWYX(State cur_state, State *result){
	if(cur_state.y > 0  &&  cur_state.x < max_x){
		result->y = max(cur_state.y - (max_x - cur_state.x), empty1);
		result->x = min(cur_state.y + cur_state.x, max_x);
		return 1;
	}
	return 0;
}
//Goi cac phep toan tren trang thai
int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pWFullX(cur_state, result);
		case 2: return pWFullY(cur_state, result);
		case 3: return pWEmptyX(cur_state, result);
		case 4: return pWEmptyY(cur_state, result);
		case 5: return pWXY(cur_state, result);
		case 6: return pWYX(cur_state, result);
		default: printf("Error calls operators");
			return 0;
	}
}
//Khai bao cau truc nut (dinh) de dung cau tim kiem
typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;
//So sanh 2 trang thai
int compareStates(State s1, State s2){
	return (s1.x == s2.x && s1.y == s2.y);
}
//Tim trang thai trong Queue Open/close
int find_State(State state, queue<Node*> openQueue){
	while (!openQueue.empty()){
		if(compareStates(openQueue.front()->state, state))
			return 1;
			openQueue.pop();
	}
	return 0;
}
//Tim kiem theo chieu rong
Node* BFS_Algorithm(State state){
	//khai bao hai queue open va close
    queue<Node*> Open_BFS;
    queue<Node*> Close_BFS;
    //Tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    if (goalcheck(root->state))
        return root;
    Open_BFS.push(root);
    while(!Open_BFS.empty()){
		//Lay mot dinh trong queue 
		Node* node = Open_BFS.front();
		Open_BFS.pop();
		Close_BFS.push(node);		 
		int opt;
		// Goi cac phep toan tren trang thai
		for(opt=1; opt<=6; opt++) {
			State newstate;
			makeNullState(&newstate);
			if(call_operator(node->state, &newstate, opt)) {
				//Neu trang thai moi sinh ra da ton tai thi bo qua
				if(find_State(newstate, Close_BFS) || find_State(newstate, Open_BFS))
					continue;				
				if(goalcheck(newstate)){
					Node *newNode = (Node*)malloc(sizeof(Node));
					newNode->state = newstate;
					newNode->Parent = node;
					newNode->no_function = opt;
					return newNode;
				}
				//Neu trang thai moi chua ton tai thi them vao queue
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				Open_BFS.push(newNode);
			}					
		}
	}
	return NULL;
}
//In ket qua chuyen nuoc de dat den trang thai muc tieu
void print_WaysToGetGoal(Node* node){
	stack<Node*> stackPrint;
	//Duyet nguoc ve nut parent
	while(node->Parent != NULL){
		stackPrint.push(node);
		node = node->Parent;
	}
	stackPrint.push(node);
	//In ra thu tu hanh dong chuyen nuoc
	int no_action = 0;
	while(!stackPrint.empty()){
		printf("\nActive %d: %s",no_action, action[stackPrint.top()->no_function]);
		print_State(stackPrint.top()->state);
		stackPrint.pop();
		no_action++;
	}
}
int main(){
	State cur_state= {0, 0};
	Node* p = BFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}
