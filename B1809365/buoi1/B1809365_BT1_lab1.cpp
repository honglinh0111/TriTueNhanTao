//BT1: Bieu dien trang thai va cac phep toan cua bai toan

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

const char* action[] = {"First State", "pour Water Full X", "pour Water Full Y",
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

//pW:pourWater		; cur_state: trang thai hien tai cua binh
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

int main(){
	State cur_state = {5, 4}, result;
	printf("Trang thai bat dau");
	print_State(cur_state);
	int opt;
	for(opt=1; opt<=6; opt++){
		int thuchien = call_operator(cur_state, &result, opt);
		if(thuchien == 1){
			printf("\nHanh dong %s thanh cong", action[opt]);
			print_State(result);
		}
		else
			printf("\nHanh dong %s KHONG thanh cong", action[opt]);
	}
	return 0;
}
