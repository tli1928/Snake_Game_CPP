/*game.hpp -------
*
* Filename: game.hpp
* Description:
* Author: Adeel Bhutta
* Maintainer:
* Created: Sat Sep 12 13:16:12 2022
* Last-Updated: September 12 16:51 2022
*
*/

/* Commentary:r
*
*
*
*/

/* Change log:
*
*
*/

/*Copyright (c) 2022 Adeel Bhutta
*
* All rights reserved.
*
* Additional copyrights may follow
*/
struct Obstacle {
  int x;
  int y;
  struct Obstacle* next;
};

typedef struct Obstacle Obstacle; 

void game();
void generate_points(int* food_x, int* food_y, int width, int height, int x_offset, int y_offset);
enum State{INIT, ALIVE, DEAD, PAUSE, EXIT};

void save_score(int score);

void generate_ob_points(int *obstacle_x, int *obstacle_y, int width, int height, int x_offset, int y_offset);
Obstacle* create_obstacle(int x, int y);
Obstacle* rand_obstacle(Obstacle* obstacles, int obstacle_x, int obstacle_y);
bool obstacle_exists(Obstacle *obstacles, int x, int y);
void add_new_obstacle(Obstacle *obstacles, Obstacle *new_obstacle);
void draw_obstacle(Obstacle* obstacle);
