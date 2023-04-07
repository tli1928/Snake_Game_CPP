/*food.cpp -------
 *
 * Filename: food.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Sat Sep 12 13:16:12 2022
 * Last-Updated: September 12 16:51 2022
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

/*Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include "food.hpp"
#include <cstdbool>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

// Create new food
Food *create_food(int x, int y, enum Type type) {
  Food *new_food = (Food *)malloc(sizeof(Food));
  char food_type;

  new_food->x = x;
  new_food->y = y;
  if (type == Increase) {
    food_type = rand() % 2 == 0 ? 'O' : '+';
  } else if (type == Decrease) {
    food_type = rand() % 2 == 0 ? 'X' : '-';
  }
  new_food->type = food_type;
  new_food->next = NULL;

  return new_food;
}

// Check if food exists at coordinates
bool food_exists(Food *foods, int x, int y) {
  Food *temp = foods;
  while (temp) {
    if (temp->x == x && temp->y == y) {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

// Add new food to end of food list
void add_new_food(Food *foods, Food *new_food) {
  Food *temp = foods;
  while (temp->next) {
    temp = temp->next;
  }
  temp->next = new_food;
}

enum Type food_type(Food *foods, int x, int y) {
  // Implement the code to return the type of the food
  // present at position (x, y)
  Food *temp = foods;
  Type type;

  while (temp->next != NULL) {

    if (temp->x == x && temp->y == y) {
      if (temp->type == 'O' || temp->type == '+') {
        type = Increase;
      } else if (temp->type == 'X' || temp->type == '-') {
        type = Decrease;
      }
    }
    temp = temp->next;
  }
  return type;
}

Food *remove_eaten_food(Food *foods, int x, int y) {
  // Implement the code to remove food at position (x,y).
  // Create a new linked list of type Food containing only the
  // needed food and return this new list
  Food *temp = foods;
  Food *pre = NULL;

  while (temp->next != NULL) {

    if (temp->x == x && temp->y == y) {

      if (pre == NULL) {
        foods = temp->next;
        break;
      } 
      else if (pre != NULL) {
        pre->next = temp->next;
        break;
      }
    }
    pre = temp;
    temp = temp->next;
  }
  return foods;
}

// Display all the food
void draw_food(Food *foods) {
  init_pair(3, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(3));
  Food *temp = foods;
  while (temp) {
    mvprintw(temp->y, temp->x, "%c", temp->type);
    temp = temp->next;
  }
  attroff(COLOR_PAIR(3));
}
