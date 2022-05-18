//Ho Ten: Pham Thi Hong Linh
//MSSV : B1809365
//STT : 45

//cau a: in ra trang thai co the co cua trang thai hien tai

#include <stdio.h>
#include <stdlib.h>

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

//Robot o vi tri ban dau
void makenull_state(State *state){
	state->robot_x = 0;
	state->robot_y = 0;
}

//in toa do cua con robot
void print_State(State state){
	printf(" (X , Y) = (%d , %d)",state.robot_x,state.robot_y);
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

//Cau truc mot node nhu sau
typedef struct Node{
	State state;
	struct Node* parent;
	int no_function; //hanh dong sinh ra trang thai hien tai
} Node;

int main(){
	State cur_state = {0, 0}, result;
	printf("Trang thai bat dau");
	print_State(cur_state);
	int opt;
	for(opt=1; opt<=4; opt++){
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
