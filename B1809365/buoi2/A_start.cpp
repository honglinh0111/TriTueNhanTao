/*
	Ho va ten: Huynh Quang Hung
	MSSV: B1809351
	Nhom: 01
*/
#include<stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <cmath>
#include <set>>
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

int h1(State s, State s2){
	int count = 0;
	for (int row = 0 ; row < BOARD_ROW ; row++)
		for (int col = 0 ; col < BOARD_COL ; col++){
			if (s.num[row][col] != s2.num[row][col])
				count++;
		}
	return count;
}

Node *find_node(State s,multiset<Node*,node_cmp> list){
	for (Node* n: list){
		if (sameState(s,n->state))
			return n;
	}
	return NULL;
}

bool find_state(State s,vector <State> *explored){
	for (State c1: *explored){
		if (sameState(s,c1))
			return true;
	}
}

Node *A_star(State init_state, State goal_state, vector<State> *explored){
	Node *root = new Node();
	root->state = init_state;
	root->parent=NULL;
	root->g =0;
	root->h=h1(init_state,goal_state);
	root->f=root->g + root->h;
	multiset<Node*,node_cmp> frontiers;
	frontiers.insert(root);
	while(!frontiers.empty()){
		Node *node = *frontiers.begin();
		frontiers.erase(frontiers.begin());
		explored->push_back(node->state);
		if (sameState(node->state, goal_state))
			return node;
		for (int op = 1; op <= MAX_OP ; op++){
			State new_state;
			if (call_operator(node->state, new_state,op)){
				if (find_state(new_state,explored))
					continue;
				Node *n = find_node(new_state,frontiers);
				if (n==NULL){
					n = new Node();
					n->parent=node;
					n->state =new_state;
					n->h =h1(new_state,goal_state);
					print_state(new_state);
					n->g=node->g+1;
					n->f=n->h+n->g;
					printf("=== Gia tri g: %d === Gia tri h: %d\n\n",n->g,n->h);
					frontiers.insert(n);
				}
				else{
					n->g=node->g+1;
					n->f=n->g+n->h;
				}
			}
		}
	}
	return NULL;
}

void print_path(Node *r){
	int i=0;
	stack<State>q;
	
	while(r->parent != NULL){
		q.push(r->state);
		r=r->parent;
	}
	q.push(r->state);
	while(!q.empty()){
		printf("Trang thai thu %d\n",i);
		i++;
		print_state(q.top());
		q.pop();
	}
}
int main(){
	freopen("text.txt","r",stdin);
	
	printf("Trang thai bat dau: \n");
	State *start = getState();
	print_state(*start);
	
	printf("Trang thai muc tieu: \n");
	State *goal = getState();
	print_state(*goal);
	
	printf("-----------------------\n");
	
	vector<State> explored_h1;
	Node* n= A_star(*start,*goal,&explored_h1);
	print_path(n);
		
}
