#include <bits/stdc++.h>

using namespace std;

struct State {
  int pl, tu;
  vector< int > st;
  State(){
  };
  bool operator < (State o) {
    if(pl != o.pl) return pl < o.pl;
    if(tu != o.tu) return tu < o.tu;
    return st < o.st;
  }
};

const int N = (int)5;
const int M = (int)5;

int n, t, m; //number of pile, number of different time states, number of turns
int h[N]; //h[i] = length of pile i;
double r[N][M]; //r[i][j] = reward to insert a stone on pile i using j seconds;
double br[N]; //br[i] = bonus reward to fill the i-th pile;
double q[N][M]; //q[i][j] = probability of lose a stone in pile i spend j seconds;


int main() {
  scanf("%d %d %d", &n, &t, &m);
  for(int i = 0; i < n; ++i) scanf("%d", h + i); // read size
  for(int i = 0; i < n; ++i) scanf("%lf", br + i); // read bonus
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      scanf(" %lf", &r[i][j]); //read reward
    }
  }
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      scanf(" %lf", &q[i][j]); //read probability
    }
  }

  State atualState;
  

  return 0;
}