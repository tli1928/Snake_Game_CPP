/*obstacle.cpp -------
*
* Filename: obstacle.cpp
* Description:
* Author: Teng Li & Zhaorui Zhang
* Maintainer:
* Created: Thur Feb 23 14:32:26 2023
* Last-Updated: 
*
*/

#include "obstacle.hpp"
#include <cstdbool>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Obstacle* create_obstacle(int x, int y) {
  
  Obstacle* new_obstacle = (Obstacle *)malloc(sizeof(Obstacle));
  new_obstacle->x = x;
  new_obstacle->y = y;
  /*
  // seed the random number generator with current time
  srand(time(0)); 
  int rows, cols;

  // randomly generate matrix size
  int matrix_size = rand() % 4 + 1; 

  
  // randomly generate single dimension matrix
  char new_obstacle[4];
  for (int i = 0; i < matrix_size; i++){
    new_obstacle[i] = '$';
  }
  */
  
  return new_obstacle;
}
/*
// Check if obstacle exists at coordinates
bool obstacle_exists(Obstacle *obstacle, int x, int y) {
  Obstacle *temp = obstacle;
  while (temp) {
    if (temp->x == x && temp->y == y) {
      return true;
    }
    temp = temp->next;
  }
  return false;
}
*/
// Display all the food
void draw_obstacle(Obstacle *obstacle) {
  init_pair(3, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(3));

  Obstacle *temp = obstacle;
  while (temp) {
    mvprintw(temp->y, temp->x, "%c", '$');
    temp = temp->next;
  }
  attroff(COLOR_PAIR(3));
}
