/* snake.cpp -------
 *
 * Filename: snake.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Sun Sep 13 9:12:30 2022
 * Last-Updated: September 13 22:40 2022
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include "snake.hpp"
#include "key.hpp"
#include <cstdbool>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

// Initialize snake facing right
Snake *init_snake_right(int x, int y) {
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x - 1, y);
  Snake *tail2 = create_tail(x - 2, y);
  tail1->next = tail2;
  head->next = tail1;

  return head;
}

// Initialize snake facing left
Snake *init_snake_left(int x, int y) {
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x + 1, y);
  Snake *tail2 = create_tail(x + 2, y);
  tail1->next = tail2;
  head->next = tail1;

  return head;
}

// Creates one tail
Snake *create_tail(int x, int y) {
  Snake *snake = (Snake *)malloc(sizeof(snake));
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 255;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

// Moves the snake in the input direction
Snake *move_snake(Snake *snake, int direction) {
  // TODO
  Snake *new_head = (Snake *)malloc(sizeof(snake));
  new_head->x = snake->x;
  new_head->y = snake->y;

  // Set the new head to have the x and y coordinates as the existing head of
  // the snake
  switch (direction) {
  case UP:
    // Write code to make the new head go up by 1 cell
    new_head->x = snake->x;
    new_head->y = snake->y - 1;
    break;
  case DOWN:
    // Write code to make the new head go down by 1 cell
    new_head->x = snake->x;
    new_head->y = snake->y + 1;
    break;
  case RIGHT:
    // Write code to make the new head go right by 1 cell
    new_head->x = snake->x + 1;
    new_head->y = snake->y;
    break;
  case LEFT:
    // Write code to make the new head go left by 1 cell
    new_head->x = snake->x - 1;
    new_head->y = snake->y;
    break;
  }

  // Set new head as the new head of the entire snake
  // Add all the features (color and symbol) to the new cell
  // Delete the tail from the snake: HINT - there is a remove tail function
  // below
  new_head->color[0] = snake->color[0];
  new_head->color[1] = snake->color[1];
  new_head->color[2] = snake->color[2];
  new_head->speed = snake->speed;
  new_head->symbol = '#';
  new_head->next = snake;
  remove_tail(new_head);

  return new_head;
}

Snake* remove_tail(Snake *snake) {
  Snake* end = snake;
  while (end->next->next)
    end = end->next;
  free(end->next);
  end->next = NULL;
  return snake;
}

// draws the snake on the board
void draw_snake(Snake *snake) {
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  attron(COLOR_PAIR(2));
  while (snake) {
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
  }
  attroff(COLOR_PAIR(2));
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake *snake) {
  // TODO for Milestone 2 only
  if (snake->next == NULL) {
    return false;
  }
  
  Snake *temp = snake->next;
  bool result = false;

  while (temp->next != NULL) {
    if (snake->x == temp->x && snake->y == temp->y) {
      result = true;
      break;
    } else {
      temp = temp->next;
    }
  }
  return result;
}

bool hit_wall(Snake *snake) {
  chtype ch[1];
  int x = snake->x;
  int y = snake->y;
  
  mvinchnstr(y, x, ch, 1);
  
  return (ch[0] & A_CHARTEXT) == '*' || (ch[0] & A_CHARTEXT) == '$';
}
