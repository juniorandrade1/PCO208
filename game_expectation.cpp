#include <bits/stdc++.h>

using namespace std;

struct State {
  int pl, tu;
  vector< int > st;
  State(){
  };
  bool operator < (State o) const {
    if(pl != o.pl) return pl < o.pl;
    if(tu != o.tu) return tu < o.tu;
    return st < o.st;
  }

  void printState() {
    printf("player = %d\n", pl);
    printf("turn = %d\n", tu);
    printf("st =");
    for(int i = 0; i < st.size(); ++i) printf(" %d", st[i]);
    printf("\n");
  }
};

const int N = (int)5; //Maximum number of piles
const int M = (int)5; //Maximum number of different times

int n, t, m; //number of pile, number of different time states, number of turns
int h[N]; //h[i] = length of pile i;
double r[N][M]; //r[i][j] = reward to insert a stone on pile i using j seconds;
double br[N]; //br[i] = bonus reward to fill the i-th pile;
double q[N][M]; //q[i][j] = probability of lose a stone in pile i spend j seconds; the sum of each row in this matrix must be less or equal to 1.0

map< State, double > memo;

double f(State S) {
  //base case
  if(S.tu == m) return 0;
  if(memo.find(S) != memo.end()) return memo[S];
  double ans;
  if(S.pl == 0) {
    ans = -HUGE_VAL;
    for(int i = 0; i < n; ++i) { //choose action that maximize
      if(S.st[i] == h[i]) continue;
      double foo = 0;
      double probSum = 0;
      for(int j = 0; j < t; ++j) {
        probSum += q[i][j];
        State Swin = S; Swin.pl ^= 1;
        State SLose = S; SLose.pl ^= 1;
        double rwin = r[i][j], rlose = r[i][j];
        Swin.st[i]++;
        if(Swin.st[i] == h[i]) rwin += br[i];
        double ax = (1.0 - probSum) * (f(Swin) + rwin) + probSum * (f(SLose) + rlose);
        foo += ax;
      }
      ans = max(ans, foo);
    }
  }
  else {
    ans = HUGE_VAL;
    for(int i = 0; i < n; ++i) { //choose action that minimize
      if(S.st[i] == h[i]) continue;
      double foo = 0;
      double probSum = 0;
      for(int j = 0; j < t; ++j) {
        probSum += q[i][j];
        State Swin = S; Swin.pl ^= 1;
        State SLose = S; SLose.pl ^= 1;
        double rwin = r[i][j], rlose = r[i][j];
        Swin.st[i]++;
        if(Swin.st[i] == h[i]) rwin += br[i];
        Swin.tu++;
        SLose.tu++;
        double ax = (1.0 - probSum) * (f(Swin) - rwin) + probSum * (f(SLose) - rlose);
        foo += ax;
      }
      ans = min(ans, foo);
    }
  }
  if(ans == HUGE_VAL || ans == -HUGE_VAL) ans = 0;
  //S.printState();
  //printf("%.10lf\n\n\n", ans);
  return memo[S] = ans;
}

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
  State atual;
  atual.pl = atual.tu = 0; atual.st = vector< int >(n, 0);
  double s = f(atual);
  printf("%.10lf\n", s);

  /*
  for(int i = 0; i < turn; ++i) {

  }
  */
  return 0;
}