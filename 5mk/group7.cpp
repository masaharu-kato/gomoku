#include "groups.h"


//=======================================================
// Group 7
//=======================================================

#include<set>
#include<algorithm>
#include<iostream>
using namespace std;

class str7_Gomoku{
public:
	str7_Gomoku();
	~str7_Gomoku();
	bool hit(int y, int x, int color);//置いた場所の判定で勝敗が決まるか判定
	bool hit(int y, int x);
	bool checkHit(int y, int x)const; //おけなければfalse
	void print()const;
	int getColor(int y, int x);
	int getNowColor()const;
private:
	int color;
	bool judge(int y, int x);//置いた場所の判定で勝敗が決まるか判定
	int table[10][10]; //y x
};

class str7_AI{
public:
	str7_AI();
	str7_AI(int color);
	~str7_AI();
	std::pair<int, int> getHit(str7_Gomoku gomoku);
	double eval(str7_Gomoku &gomoku);
	double eval(str7_Gomoku &gomoku, int color);
//private:
        int dfsab(str7_Gomoku &g,int color,int depth,int alph,int beta);
  //int AI::dfsab(Gomoku &g,int color,int depth,int alph,int beta);
	int dfs(str7_Gomoku &g, int color, int depth);
	int getConnectN(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getPos(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getPriorityPos(str7_Gomoku &gomoku, int color);
	std::set<std::pair<int, int> > getAround(str7_Gomoku &gomoku);
	int getEnemyColor();
	int getEnemyColor(int color);
	int myColor;
};




str7_Gomoku::str7_Gomoku(){
	color = 1;
	for (int i = 0; i<10; ++i){
		for (int j = 0; j<10; ++j){
			table[i][j] = 0;
		}
	}
}

str7_Gomoku::~str7_Gomoku(){

}

bool str7_Gomoku::hit(int y, int x, int color){
	table[y][x] = color;
	return judge(y, x);
}

bool str7_Gomoku::hit(int y, int x){
	table[y][x] = color;
	//	cout << table[y][x] <<" color:"<<color <<endl;
	if (color == 1) color = 2;
	else color = 1;
	return judge(y, x);
}

bool str7_Gomoku::judge(int y, int x){
	const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int co = table[y][x];
	for (int i = 0; i<4; ++i){
		int c = 1;
		int ny = y;
		int nx = x;
		for (int j = 0; j<4; ++j){
			ny = ny + dy[i];
			nx = nx + dx[i];
			if (0 <= ny && ny <= 9 && 0 <= nx && nx <= 9){
				if (table[ny][nx] == co) c++;
				else break;
			}
		}
		ny = y;
		nx = x;
		for (int j = 0; j<4; ++j){
			ny = ny + dy[i + 4];
			nx = nx + dx[i + 4];
			if (0 <= ny && ny <= 9 && 0 <= nx && nx <= 9){
				if (table[ny][nx] == co) c++;
				else break;
			}

		}
		//		cout << c <<endl;
		if (c >= 5) return true;
	}
	return false;
}

bool str7_Gomoku::checkHit(int y, int x)const{
	if (y< 0 || 9< y || x< 0 || 9< x) return false;
	if (table[y][x] != 0) return false;
	return true;
}

void str7_Gomoku::print()const{
	cout << "  ";
	for (int i = 0; i<10; ++i) cout << i << " ";
	cout << endl;
	for (int i = 0; i<10; ++i){
		cout << i << " ";
		for (int j = 0; j<10; ++j){
			if (table[i][j] == 0)cout << "  ";
			else if (table[i][j] == 1) cout << "Ｘ ";
			else  cout << "◯ ";
			//		else cout << table[i][j] << "  ";
		}cout << endl;
	}
}

int str7_Gomoku::getColor(int y, int x){
	return table[y][x];
}

int str7_Gomoku::getNowColor()const{
	return color;
}





bool str7_onBoard(int y, int x){
	if (0 <= y && y < 10 && 0 <= x && x< 10)
		return true;

	return false;
}

str7_AI::str7_AI(){}

str7_AI::str7_AI(int color){
	myColor = color;
}

str7_AI::~str7_AI(){}


set<pair<int, int> > str7_AI::getPos(str7_Gomoku &gomoku, int color){
	set<pair<int, int> > res;
	res = getPriorityPos(gomoku, color);
	if (!res.empty()) return res;
	res = getAround(gomoku);
	if (!res.empty()) return res;
	res.insert(pair<int, int>(4, 4));
	return res;
}

set<pair<int, int> > str7_AI::getAround(str7_Gomoku &gomoku){
	set<pair<int, int> > se;

	const int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	for (int i = 0; i<10; ++i){
		for (int j = 0; j<10; ++j){
			if (gomoku.getColor(i, j) != 0){
				for (int ddx = 0; ddx<8; ++ddx){
					for (int ddy = 0; ddy<8; ++ddy){
						int xx = j + dx[ddx];
						int yy = i + dy[ddy];
						if (gomoku.checkHit(yy, xx) == true) se.insert(pair<int, int>(yy, xx));
	/*					xx = j + dx[ddx] *2;
						yy = i + dy[ddy] *2;
						if(gomoku.checkHit(yy,xx) == true) se.insert(pair<int,int>(yy,xx));
						*/
					}
				}
			}
		}
	}

	return se;
}


//置いたら勝ち
//置かれたら負け
//自分が4のフリー作れる
//相手が４のフリー作れる

//置いたら勝ち(4つで一つ空き	0
//相手が4						1
//自分が3						2
//相手が3						3 
set<pair<int, int> > str7_AI::getPriorityPos(str7_Gomoku &gomoku, int myCol){
	set<pair<int, int> > res;
	int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	set<pair<int, int> > tmp[4];
	for (int y = 0; y<10; ++y){
		for (int x = 0; x<10; ++x){
			int color = gomoku.getColor(y, x);
			if (color == 0){
				for (int c = 1; c<3; ++c){
					if (gomoku.hit(y, x, c)){
						int index = 1;
						if (c == myCol) index = 0;
						tmp[index].insert(pair<int, int>(y, x));
						//		break;
					}
					else{
						for (int d = 0; d<4; ++d){
							int deathPoint = 0;
							int connect = 1;
							int ny = y + dy[d];
							int nx = x + dx[d];

							while (str7_onBoard(ny, nx) &&
								gomoku.getColor(ny, nx) == gomoku.getColor(y, x)){
								connect++;
								ny += dy[d];
								nx += dx[d];
							}
							if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == 0))
								deathPoint++;

							ny = y + dy[d + 4];
							nx = x + dx[d + 4];
							while (str7_onBoard(ny, nx) &&
								gomoku.getColor(ny, nx) == gomoku.getColor(y, x)){
								ny += dy[d + 4];
								nx += dx[d + 4];
								connect++;
							}

							if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == 0))
								deathPoint++;

							if (connect == 4 && deathPoint == 0){
								int index = 3;
								if (c == myCol) index = 2;
								tmp[index].insert(pair<int, int>(y, x));
								//				break;
							}
							//				cout << "(" << y << "," << x << ","<< connect<<"," << deathPoint<< ")" << endl;

						}
					}
					gomoku.hit(y, x, 0);
				}
			}

		}
	}
	for (int i = 0; i<4; ++i){
		if (!res.empty()) break;
		set<pair<int, int> >::iterator it = tmp[i].begin();
		while (it != tmp[i].end()){
			res.insert(*it);
			//		cout << i << endl;
			it++;
		}
	}
	return res;
}
	
int str7_AI::dfsab(str7_Gomoku &g,int color,int depth,int alph,int beta){
	if(depth == 4) return (int)eval(g,myColor);
	set<pair<int,int> > possible = getPos(g,getEnemyColor(color));
	set<pair<int,int> >::iterator it = possible.begin();
	int res = alph;
//	if(color != myColor) res  = beta;
	while(it != possible.end()){
		int turnColor = getEnemyColor(color);
		bool f = g.hit(it->first,it->second,turnColor);
		int tmp;
		if(f == true){
			g.hit(it->first,it->second,0);
			if(turnColor == myColor) return 100000;
			else return -100000;
		} 
		tmp = dfsab(g,turnColor,depth+1,alph,beta);
		g.hit(it->first,it->second,0);
		
		if(color != myColor){
			alph = max(alph,tmp);
			res = alph;
			if(res >= beta) return beta;
/*			if(res < tmp){
				res = tmp;
			}*/
		}else{
			beta = min(beta,tmp);
			res = beta;
			if(beta <= alph) return alph;
//			if(res > tmp) res = tmp;
		
		}
		it++;
	}
	return res;		

}


int str7_AI::dfs(str7_Gomoku &g, int color, int depth){
	if (depth == 3) return (int)eval(g, myColor);
	set<pair<int, int> > possible = getPos(g, getEnemyColor(color));
	//	g.print();	
	set<pair<int, int> >::iterator it = possible.begin();
	int res = 1000000;
	if (color != myColor) res = -100000000;
	while (it != possible.end()){
		int turnColor = getEnemyColor(color);
		bool f = g.hit(it->first, it->second, turnColor);
		int tmp;
		if (f == true){
			g.hit(it->first, it->second, 0);
			if (turnColor == myColor) return 100000;
			else return -100000;
			/*			if(enemyColor == myColor) tmp = 100000;
			else tmp =  -100000;
			*/
		}
		tmp = dfs(g, turnColor, depth + 1);
		g.hit(it->first, it->second, 0);
		//		if(depth == 0 ) cout << "(" <<it->first<< "," << it->second << ")"<< tmp << " ";
		if (color != myColor){
			if (res < tmp){
				res = tmp;
			}
		}
		else{
			if (res > tmp) res = tmp;

		}
		it++;
	}
	return res;
}


pair<int, int> str7_AI::getHit(str7_Gomoku g){
	set<pair<int,int> > possible = getPos(g,myColor);
	set<pair<int,int> >::iterator it = possible.begin();
	int res = -10000009;
	pair<int,int> resP(0,0);
	
	while(it != possible.end()){
		
		bool f = g.hit(it->first,it->second,myColor);
		if(f){
			g.hit(it->first,it->second,0);
			return pair<int,int>(it->first,it->second);
		}
//		cout << "dfs" << it->first << "," << it->second << endl;
//		int tmp = dfs(g,myColor,0);
		int tmp = dfsab(g,myColor,0,res,9999999);
		g.hit(it->first,it->second,0);
		if(res < tmp){
			resP =  *it;
			res = tmp;
		}
	//	cout << "("<<it->first << "," <<it->second << ") value:" << tmp <<endl;
		it++;
	}
	return resP;
	return pair<int,int>(4,4);	
}

int str7_AI::getEnemyColor(int color){
	if (color == 1) return 2;
	else return 1;
}

int str7_AI::getEnemyColor(){
	if (myColor == 1) return 2;
	return 1;
}

int str7_AI::getConnectN(str7_Gomoku &gomoku, int color){
	int res = 0;
	int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	//	set<pair<int,int> > se;
	for (int y = 0; y<10; ++y){
		for (int x = 0; x<10; ++x){
			if (gomoku.getColor(y, x) != color) continue;
			for (int d = 0; d<4; ++d){
				int deathPointN = 0;
				int connectN = 1;
				int backY = y + dy[d + 4];
				int backX = x + dx[d + 4];
				if (str7_onBoard(backY, backX)){
					if (gomoku.getColor(backY, backX) == color){
						continue;
					}
					else if (gomoku.getColor(backY, backX) == getEnemyColor(color)){
						deathPointN++;
					}
				}
				else deathPointN++;
				int nx = x + dx[d];
				int ny = y + dy[d];

				if (!(str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == color))
					continue;
				//				se.insert(pair<int,int>(y,x));
				while (str7_onBoard(ny, nx) && gomoku.getColor(ny, nx) == color){
					connectN++;
					nx += dx[d]; ny += dy[d];
				}
				if (!str7_onBoard(ny, nx) ||
					gomoku.getColor(ny, nx) == getEnemyColor(color)){
					deathPointN++;
				}

				int mul = 1;
				if (deathPointN == 2) mul = -1;
				else if (deathPointN == 0) mul = 2;

				if (connectN == 3){
					if (deathPointN == 2){
						res += (connectN + 2) * 0.8;
					}
					else
						res += (connectN + 2) * mul;
				}
				else if (connectN == 4){
					if (deathPointN == 2){
						res += (connectN + 2) * 0.8;
					}
					else if (deathPointN == 1){
						res += (connectN + 2);
					}
					else
						res += 100 * mul;
				}
				else if (connectN == 5) res += 5000;
				else res += connectN * mul;
			}
		}
	}

	return res;
}
double str7_AI::eval(str7_Gomoku &gomoku, int color){
	int my = getConnectN(gomoku, color);
	int enemy = getConnectN(gomoku, getEnemyColor(color));
	int res = int(my - 1.3*enemy);
	//	cout << "eval:" << my << ","<<enemy <<","<<res <<endl;
	return res;
	//	return getConnectN(gomoku,color) -2* getConnectN(gomoku,getEnemyColor(color));
}

double str7_AI::eval(str7_Gomoku &gomoku){
	return eval(gomoku, myColor);

}

	//------------------------------------------------
// ランダムに手を打つ戦略
//------------------------------------------------
void strategy7(const int board[][BOARD_SIZE], int *pos_x, int *pos_y, const int count, const position *history)
{
	static str7_Gomoku g;
	static int myColor = 2;
	if (count == 0) myColor = 1;
	static str7_AI ai(myColor);
//	AI(myColor);

	if (count != 0){
		int y = history[count - 1].y;
		int x = history[count - 1].x;
		g.hit(y, x);
	}

	pair<int,int> res = ai.getHit(g);
	g.hit(res.first, res.second);
	*pos_y = res.first;
	*pos_x = res.second;
	
	
	
	return;
}

