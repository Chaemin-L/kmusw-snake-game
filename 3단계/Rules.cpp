#include <ncurses.h>
#include <cstdlib>
#include "Rules.h"
#include "Item.h"
#include <vector>
using std::vector;
vector<Item> item;

// constructor : print the Head
Head::Head() {
    init();
}

void Head::init() {
    y = 10; x = 30;
    show();
}
void Head::show() {
    attron(COLOR_PAIR(3)); // head color on
    mvprintw(y, x, "3");
    attroff(COLOR_PAIR(3)); //  off
}
void Head::setposition(int i, int j) { y += i, x += j; }

// constructor : print the body
Body::Body() {
    init();
}

void Body::init() {
    len = 2; std::fill_n(x, MAXLEN, 0); std::fill_n(y, MAXLEN, 0);
    y[0] = y[1] = 10;  x[0] = 31; x[1] = 32;
    show();
}
// head에 종속적이므로, head의 위치를 인자로 받아 첫번째 body원소를 head의 위치로 이동
// 나머지 body들은 앞의 body위치로 이동
void Body::setposition(int i, int j) {
    del();
    int by, bx, py, px;   //before (y,x)position, present (y,x)position
    by = y[0]; bx = x[0];
    y[0] = i; x[0] = j;
    for (int l = 1; l < len; l++) {
        py = y[l]; px = x[l];
        y[l] = by; x[l] = bx;
        by = py; bx = px;
    }
}
void Body::show() {
    attron(COLOR_PAIR(2));  //  body color
    for (int i = 0; i < len; i++) {
        mvprintw(y[i], x[i], "4");
    }
    attroff(COLOR_PAIR(2)); //  off
}
// increase body length
void Body::IncBody() {
    // y[len-1]-y[len] = y[len-2]-y[len-1] 임을 이용.
    y[len] = 2 * y[len - 1] - y[len - 2];
    x[len] = 2 * x[len - 1] - x[len - 2];
    len++;
}
// decrease body length
void Body::DecBody() {
    mvprintw(y[len - 1], x[len - 1], " ");
    y[len - 1] = 0; x[len - 1] = 0; len--;

    if (len < 2) { Snake::failed(); }
}

// 지나간 자취 삭제
void Body::del() {
    mvprintw(y[len - 1], x[len - 1], " ");
}


bool Snake::GameOver = false;

Snake::Snake(){
  t = time(NULL);
  move();
}
void Snake::move() {
  while (GameOver != true) {
      if(time(NULL) - t > 3) {
        makeItem();
        t = time(NULL);
      }
      // 키보드 입력이 없을 때, 헤드랑 제일 가까운 body 반대방향으로 진행.
      // Head의 y좌표==Body[0]의 y좌표일 경우 서로 수평을 이루므로 x좌표 값만 변화.
      if(hd.getY()==bd.y[0]){
        if(hd.getX()>bd.x[0]) {
          keyIn(0,1);
          delay(0.4);
        }
        else {
          keyIn(0,-1);
          delay(0.4);
        }
      }
      // Head의 x좌표==Body[0]의 x좌표일 경우 서로 수직을 이루므로 y좌표 값만 변화.
      else{
        if(hd.getY()>bd.y[0]) {
          keyIn(1,0);
          delay(0.4);
        }
        else {
          keyIn(-1,0);
          delay(0.4);
        }
      }
    } //while (GameOver!=false)
} // move()
void Snake::keyIn(int y, int x) {
    // 진행방향 반대로 이동하려 하거나 body에 부딪힐때, failed 창을 띄운다.
    for (int i = 0; i < bd.len; i++) {
        if (((hd.getY() + y) == bd.y[i]) && ((hd.getX() + x) == bd.x[i])) {
            failed(); return;
        }
    }
    // 벽에 닿으면 fail
    if (((hd.getY() + y) == 0) || ((hd.getY() + y) == 20) || ((hd.getX() + x) == 0) || ((hd.getX() + x) == 59)) {
        failed(); return;
    }
    bd.setposition(hd.getY(), hd.getX());
    hd.setposition(y, x);
    itemRule();
    hd.show(); bd.show();
    refresh();
}
// delay 함수 구현 & 지연되는 시간동안 키 입력이 들어오면 움직임 제어.
int Snake::delay(float secs) {
    clock_t delay = secs * CLOCKS_PER_SEC;
    clock_t start = clock();
    while ((clock() - start) < delay){
      if(GameOver == true) break;
      char key;
      if(key=getch()){
        switch(key){
          case 'w':
            keyIn(-1,0);
            break;
          case 's':
            keyIn(1,0);
            break;
          case 'd':
            keyIn(0,1);
            break;
          case 'a':
            keyIn(0,-1);
            break;
      }
    }
  }
    return 0;
}
// failed 출력
void Snake::failed() {
    mvprintw(20, 30, "FAILED");
    refresh();
    GameOver = true;
}

// Item 생성 함수
void Snake::makeItem() {
    if (item.size() < 3) {
        srand(time(NULL));
        if (rand() % 2) {
          Growth g(hd, bd);
          Item i(g);
          item.emplace_back(i);
        }
        else {
          Poison p(hd, bd);
          Item i(p);
          item.emplace_back(i);
        }
    }
}

//void Snake::setGameStatus(bool b) { GameOver = b; }

void Snake::itemRule(){
      for(int i = item.size()-1; i >=0; i--) {
        if(item[i].rule() == -1){
          item.erase(item.begin() + i);
        }
      }
}

void Snake::newGame(){
  for(int i = item.size()-1; i >=0; i--) {
    item[i].clear();
    item.erase(item.begin() + i);
  }

  mvprintw(hd.getY(), hd.getX(), " ");
  for (int i = 0; i < bd.len; i++) {
     mvprintw(bd.y[i], bd.x[i], " ");
  }
  hd.init(); bd.init();

	attron(COLOR_PAIR(2));
  mvprintw(20, 30, "111111");
	attroff(COLOR_PAIR(2));
  refresh();

  GameOver = false;
  t = time(NULL);
  move();
}
