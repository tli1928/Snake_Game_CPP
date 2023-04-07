/*obstacle.hpp -------
*
* Filename: obstacle.hpp
* Description:
* Author: Teng Li & Zhaorui Zhang
* Maintainer:
* Created: Thur Feb 23 14:28:56 2023
* Last-Updated: 
*
*/

#include <ncurses.h>

//Structure to hold properties of obstacles
struct Obstacle {
  int x;
  int y;
  struct Obstacle* next;
};

typedef struct Obstacle Obstacle; 

//Function prototypes
// void add_new_obstacle(Obstacle* obstacle, Obstacle* new_obstacle);
// bool obstacle_exists(Obstacle* obstacle, int x, int y);
Obstacle* create_obstacle(int x, int y);
void draw_obstacle(Obstacle* obstacle);