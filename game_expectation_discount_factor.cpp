#include <bits/stdc++.h>
#include <random>

using namespace std;

struct State {
  int pl, tu, df;
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
  printf("df = %d\n", df);
  }
};  

const int N = (int)5; //Maximum number of piles
const int M = (int)5; //Maximum number of different times
const int O = (int)10;

int n, t, m; //number of pile, number of different time states, number of turns
int h[N]; //h[i] = length of pile i;
double r[N][M]; //r[i][j] = reward to insert a stone on pile i using j seconds;
double br[N]; //br[i] = bonus reward to fill the i-th pile;
double q[N][M]; //q[i][j] = probability of lose a stone in pile i spend j seconds; the sum of each row in this matrix must be less or equal to 1.0
double sq[N][M];

map< State, double > memo;

double f(State S) {
  //base case
  S.df++;
  if(S.tu == m || S.df == O) return 0;
  if(memo.find(S) != memo.end()) return memo[S];
  double ans;
  if(S.pl == 0) {
    ans = -HUGE_VAL;
    for(int i = 0; i < n; ++i) { //choose action that maximize
      if(S.st[i] == h[i]) continue;
      double probSum = 0;
      for(int j = 0; j < t; ++j) {
        probSum += q[i][j];
        State Swin = S; Swin.pl ^= 1;
        State SLose = S; SLose.pl ^= 1;
        double rwin = r[i][j], rlose = r[i][j];
        Swin.st[i]++;
        if(Swin.st[i] == h[i]) rwin += br[i];
        double ax = (1.0 - probSum) * (f(Swin) + rwin) + probSum * (f(SLose) + rlose);
        ans = max(ans, ax);
      }
    }
  }
  else {
    ans = HUGE_VAL;
    for(int i = 0; i < n; ++i) { //choose action that minimize
      if(S.st[i] == h[i]) continue;
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
        ans = min(ans, ax);
      }
    }
  }
  if(ans == HUGE_VAL || ans == -HUGE_VAL) ans = 0;
  return memo[S] = ans;
}



pair<int, int> getOptimalPlay(State S) {
  //base case
  S.df++;
  if(S.tu == m || S.df == O) return make_pair(-1, -1);
  double ans = f(S);
  if(S.pl == 0) {
    for(int i = 0; i < n; ++i) { //choose action that maximize
      if(S.st[i] == h[i]) continue;
      double probSum = 0;
      for(int j = 0; j < t; ++j) {
        probSum += q[i][j];
        State Swin = S; Swin.pl ^= 1;
        State SLose = S; SLose.pl ^= 1;
        double rwin = r[i][j], rlose = r[i][j];
        Swin.st[i]++;
        if(Swin.st[i] == h[i]) rwin += br[i];
        double ax = (1.0 - probSum) * (f(Swin) + rwin) + probSum * (f(SLose) + rlose);
        if(ans == ax) {
          printf("Optimal play is insert at pile %d at time %d and get a expected reward %.10lf\n", i, j, ans);
          return make_pair(i, j);
        }
      }
    }
  }
  else {
    for(int i = 0; i < n; ++i) { //choose action that minimize
      if(S.st[i] == h[i]) continue;
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
        if(ans == ax) {
          printf("Optimal play is insert at pile %d at time %d and get a expected reward %.10lf\n\n", i, j, ans);
          return make_pair(i, j);
        }
      }
    }
  }
  return make_pair(-1, -1);
}

int main() {
  random_device rd;
  mt19937 e2(rd());
  uniform_real_distribution<> dist(0, 1);

  FILE *file = fopen("parameters.in", "r");
  fscanf(file, "%d %d %d", &n, &t, &m);
  for(int i = 0; i < n; ++i) fscanf(file, "%d", h + i); // read size
  for(int i = 0; i < n; ++i) fscanf(file, "%lf", br + i); // read bonus
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      fscanf(file, " %lf", &r[i][j]); //read reward
    }
  }
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      fscanf(file, " %lf", &q[i][j]); //read probability
      sq[i][j] = q[i][j];
      if(j) sq[i][j] += sq[i][j - 1];
    }
  }
  puts("-----------------------------------------------------");
  printf("Number of piles: %d\n", n);
  printf("Number of different times: %d\n", t);
  printf("Number of turns: %d\n", m);
  puts("-----------------------------------------------------");
  printf("h[i] = {");
  for(int i = 0; i < n; ++i) printf(" %d", h[i]);
  printf(" }\n");
  puts("-----------------------------------------------------");
  printf("r_ij\n");
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      printf("%lf ", r[i][j]); //read reward
    }
    printf("\n");
  }
  puts("-----------------------------------------------------");
  printf("p_ij\n");
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < t; ++j) {
      printf("%lf ", q[i][j]); //read probability
    }
    printf("\n");
  }
  puts("-----------------------------------------------------");
  State atual;
  atual.pl = atual.tu = 0; atual.st = vector< int >(n, 0);
  //double s = f(atual);
  //printf("Expected reward = %.10lf\n", s);
  double ra = 0, rb = 0;
  for(int i = 0; i < m; ++i) {
    atual.df = 0;
    printf("Insert a new pile: ");
    int x, y; scanf("%d %d", &x, &y);
    if(dist(e2) >= sq[x][y]) atual.st[x]++;
    ra += r[x][y];
    if(atual.st[x] == h[x]) ra += br[x];
    atual.pl ^= 1;
    pair<int, int> o = getOptimalPlay(atual);
    printf("The second player plays %d %d\n", o.first, o.second);
    if(dist(e2) >= sq[o.first][o.second]) atual.st[o.first]++;
    rb += r[o.first][o.second];
    if(atual.st[o.first] == h[o.first]) rb += br[o.first];
    atual.tu++;
    atual.pl ^= 1;
  }
  printf("Final pontuation = %.10lf %.10lf\n", ra, rb);
  return 0;
}