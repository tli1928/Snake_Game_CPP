/*game.cpp -------
 *
 * Filename: game.cpp
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
#include "game.hpp"
#include "food.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "snake.hpp"
#include <algorithm>
#include <cstdbool>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

// Create an obstacle with the given position
Obstacle *create_obstacle(int x, int y) {
  Obstacle *new_obstacle = (Obstacle *)malloc(sizeof(Obstacle));
  new_obstacle->x = x;
  new_obstacle->y = y;
  new_obstacle->next = NULL;

  return new_obstacle;
}

// Create an obstacle with random length and direction
Obstacle* rand_obstacle(Obstacle* obstacles, int obstacle_x, int obstacle_y) {
  bool obstacle_direct = rand() % 2 == 0;
  bool obstacle_length = rand() % 2 == 0;
  
  if (obstacle_direct) {
    add_new_obstacle(obstacles, create_obstacle(obstacle_x - 1, obstacle_y));
    add_new_obstacle(obstacles, create_obstacle(obstacle_x + 1, obstacle_y));
  
    if (obstacle_length) {
      add_new_obstacle(obstacles, create_obstacle(obstacle_x - 2, obstacle_y));
      add_new_obstacle(obstacles, create_obstacle(obstacle_x + 2, obstacle_y));
    }
  }
  else if (!obstacle_direct) {
    add_new_obstacle(obstacles, create_obstacle(obstacle_x, obstacle_y - 1));
    add_new_obstacle(obstacles, create_obstacle(obstacle_x, obstacle_y + 1));
  
    if (obstacle_length) {
      add_new_obstacle(obstacles, create_obstacle(obstacle_x, obstacle_y - 2));
      add_new_obstacle(obstacles, create_obstacle(obstacle_x, obstacle_y + 2)); 
    }
  }
}

// Check if there is an obstacle on the given position
bool obstacle_exists(Obstacle *obstacles, int x, int y) {
  Obstacle* temp = obstacles;
  while (temp) {
    if (temp->x == x && temp->y == y) {
      return true;
    }
    temp = temp->next;
  }
  return false;
}

// Link the given new obstacle to the obstacles
void add_new_obstacle(Obstacle *obstacles, Obstacle *new_obstacle) {
  Obstacle *temp = obstacles;
  while (temp->next) {
    temp = temp->next;
  }
  temp->next = new_obstacle;
}

// Draw the obstacles on the canvas
void draw_obstacle(Obstacle *obstacle) {
  init_pair(4, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(4));
  Obstacle *temp = obstacle;
  while (temp) {
    mvprintw(temp->y, temp->x, "%c", '$');
    temp = temp->next;
  }
  attroff(COLOR_PAIR(4));
}

// Save the top-10 scores in the file
void save_score(int score) {
  const char* dir_path = "./saves";
  DIR* dir = opendir(dir_path);

  if (dir == NULL) {
    std::string dir_name = "./saves";
    int status = mkdir(dir_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  }
  
  std::string filepath = "./saves/save_best_10.game";
  std::ifstream infile(filepath);

  // Save the score in the file and sort them in a descending order
  if (infile.good()) {
    std::ofstream outfile(filepath, std::ios::app);
    outfile << score << std::endl;

    std::vector<int> numbers;
    int n;
    while (infile >> n) {
      numbers.push_back(n);
    }

    std::sort(numbers.rbegin(), numbers.rend());

    std::ofstream new_outfile(filepath);
    for (int number : numbers) {
      new_outfile << number << std::endl;
    }
  } else {
    std::ofstream outfile(filepath);
    outfile << score << std::endl;
  }

  // Check if there are more than 10 lines in the file
  std::ifstream new_infile(filepath);
  std::ofstream out("temp.txt");
  int line_count = 0;
  std::string line;
  while (std::getline(new_infile, line)) {
    line_count++;
    if (line_count == 11) {
      continue;
    }
    out << line << std::endl;
  }
  std::remove(filepath.c_str());
  std::rename("temp.txt", filepath.c_str());
}

void generate_points(int *food_x, int *food_y, int width, int height,
                     int x_offset, int y_offset) {
  *food_x = (rand() % (width - 1) + x_offset) + 1;
  *food_y = (rand() % (height - 1) + y_offset) + 1;
}

void generate_ob_points(int *obstacle_x, int *obstacle_y, int width, int height,
                        int x_offset, int y_offset) {
  *obstacle_x = (rand() % (width - 6) + x_offset) + 4;
  *obstacle_y = (rand() % (height - 6) + y_offset) + 4;
}

void game() {
  enum State state = INIT;           // Set the initial state
  static int x_max, y_max;           // Max screen size variables
  static int x_offset, y_offset;     // distance between the top left corner of your
                                     // screen and the start of the board
  gamewindow_t *window;              // Name of the board
  Snake *snake;                      // The snake
  Food *foods, *new_food;            // List of foods (Not an array)
  Obstacle *obstacles, *new_obstacle;// List of obstacles (Not an array)

  const int height = 30;
  const int width = 70;
  char ch;

  int key;
  int score = 0;
  double speed = 1;
  int temp_score = 0;
  int lives = 3;
  int food_amount;
  int obstacle_amount;

  bool is_start = false;              // If the game is start
  bool has_dead = false;              // If the player has dead before
  bool is_dead = false;               // If the game is dead

  struct timespec timeret;
  timeret.tv_sec = 0;
  timeret.tv_nsec = 999999999 / 4;

  while (state != EXIT) {
    switch (state) {
    case INIT:
      initscr();
      start_color();
      nodelay(stdscr, TRUE); // Dont wait for char
      noecho();              // Don't echo input chars
      getmaxyx(stdscr, y_max, x_max);
      keypad(stdscr, TRUE); // making keys work
      curs_set(0);          // hide cursor
      timeout(100);

      // Setting height and width of the board
      x_offset = (x_max / 2) - (width / 2);
      y_offset = (y_max / 2) - (height / 2);

      // Init board
      window = init_GameWindow(x_offset, y_offset, width, height);

      // Init snake
      srand(time(0));
      if (rand() % 2 == 0) {
        snake =
            init_snake_right(x_offset + (width / 2), y_offset + (height / 2));
        key = RIGHT;
      } else {
        snake =
            init_snake_left(x_offset + (width / 2), y_offset + (height / 2));
        key = LEFT;
      }

      // Welcome window
      ch = get_char();

      if (!is_start) {
        mvprintw(y_max / 2 - 3, x_max / 2 - 13, "Welcome to the Snake Game!");
        mvprintw(y_max / 2 - 2, x_max / 2 - 22,
                 "Controls: Use arrow keys to move the snake.");
        mvprintw(
            y_max / 2 - 1, x_max / 2 - 30,
            "Scoring System: 'o' gives 20 points and 'x' minus 10 points.");
        mvprintw(y_max / 2, x_max / 2 - 38,
                 "You will get a speed up for every time your speed-up points "
                 "reached to 100!");
        mvprintw(y_max / 2 + 1, x_max / 2 - 16,
                 "You lose when your body is gone!");
        mvprintw(y_max / 2 + 3, x_max / 2 - 14, "Press 'S' to start the game.");
      }

      // Start the game
      if (ch == 's' || ch == 'S') {
        is_start = true;
      }

      if (is_start && !has_dead) {
        clear();
        mvprintw(y_max / 2 - 1, x_max / 2 - 13, "Press 'E' for easy mode.");
        mvprintw(y_max / 2, x_max / 2 - 13, "Press 'H' for hard mode.");
        mvprintw(y_max / 2 + 1, x_max / 2 - 13, "Press 'A' for advanced mode.");

        if (ch == 'e' || ch == 'E') {
          obstacle_amount = 3;
          food_amount = 10;
          state = ALIVE;
        }
        if (ch == 'h' || ch == 'H') {
          timeret.tv_nsec = timeret.tv_nsec / 2;
          obstacle_amount = 6;
          food_amount = 6;
          state = ALIVE;
        }
        if (ch == 'a' || ch == 'A') {
          timeret.tv_nsec = timeret.tv_nsec / 4;
          obstacle_amount = 9;
          food_amount = 3;
          state = ALIVE;
        }
      }

      // Generate obstacles by the difficulty
      int obstacle_x, obstacle_y, n;
      
      generate_ob_points(&obstacle_x, &obstacle_y, width, height, x_offset,
                         y_offset);
      obstacles = create_obstacle(obstacle_x, obstacle_y);
      rand_obstacle(obstacles, obstacle_x, obstacle_y);
      
      for (n = 1; n < obstacle_amount; n++) {
        generate_ob_points(&obstacle_x, &obstacle_y, width, height, x_offset,
                           y_offset);
        while (obstacle_exists(obstacles, obstacle_x, obstacle_y))
          generate_ob_points(&obstacle_x, &obstacle_y, width, height, x_offset, y_offset);
        rand_obstacle(obstacles, obstacle_x, obstacle_y);
        new_obstacle = create_obstacle(obstacle_x, obstacle_y);
        add_new_obstacle(obstacles, new_obstacle);
      }
      
      // Init foods
      int food_x, food_y, i;
      enum Type type;

      // Generate foods by the difficulty
      generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
      type = (rand() > RAND_MAX / 2)
                 ? Increase
                 : Decrease; // Randomly deciding type of food
      foods = create_food(food_x, food_y, type);
      for (i = 1; i < food_amount; i++) {
        generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        while (food_exists(foods, food_x, food_y) || obstacle_exists(obstacles, food_x, food_y))
          generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        type = (rand() > RAND_MAX / 2) ? Increase : Decrease;
        new_food = create_food(food_x, food_y, type);
        add_new_food(foods, new_food);
      }

      if (has_dead) {
        clear();
        mvprintw(y_max / 2, x_max / 2 - 14, "Lives left: %d", lives);
        mvprintw(y_max / 2 + 2, x_max / 2 - 14, "Press 'S' to start again.");

        if (ch == 's' || ch == 'S') {
          is_dead = false;
          state = ALIVE;
        }
      }

      if (ch == 'q' || ch == 'Q') {
        state = EXIT;
        break;
      }
      break;

    case ALIVE:
      ch = get_char();

      /* Write your code here */
      if (ch == UP || ch == DOWN || ch == LEFT || ch == RIGHT) {
        if (key == UP && ch == DOWN) {
          key = UP;
        } else if (key == DOWN && ch == UP) {
          key = DOWN;
        } else if (key == LEFT && ch == RIGHT) {
          key = LEFT;
        } else if (key == RIGHT && ch == LEFT) {
          key = RIGHT;
        } else {
          key = ch;
        }
      }
      snake = move_snake(snake, key);

      if (is_dead) {
        lives--;
        if (lives == 0) {
          state = DEAD;
        } else {
          state = INIT;
        }
      }

      if (hit_wall(snake)) {
        has_dead = true;
        is_dead = true;
      }

      if (eat_itself(snake)) {
        has_dead = true;
        is_dead = true;
      }

      if (food_exists(foods, snake->x, snake->y)) {

        if (food_type(foods, snake->x, snake->y) == Increase) {
          score += 20;
          temp_score += 20;

          Snake *new_tail;

          if (key == UP) {
            new_tail = create_tail(snake->x, snake->y + 1);
          }

          if (key == DOWN) {
            new_tail = create_tail(snake->x, snake->y - 1);
          }

          if (key == LEFT) {
            new_tail = create_tail(snake->x + 1, snake->y);
          }

          if (key == RIGHT) {
            new_tail = create_tail(snake->x - 1, snake->y);
          }
          new_tail->next = snake->next;
          snake->next = new_tail;
        } else if (food_type(foods, snake->x, snake->y) == Decrease) {
          score -= 10;

          if (temp_score > 0) {
            temp_score -= 10;
          }

          if (snake->next != NULL) {
            remove_tail(snake);
          } else {
            has_dead = true;
            is_dead = true;
          }
        }

        foods = remove_eaten_food(foods, snake->x, snake->y);

        if (temp_score >= 100) {
          temp_score -= 100;
          timeret.tv_nsec /= 1.5;
          speed *= 1.5;
        }

        int food_x, food_y, i;
        enum Type type;
        Food *new_food;

        generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        type = (rand() > RAND_MAX / 2)
                   ? Increase
                   : Decrease; // Randomly deciding type of food
        while (food_exists(foods, food_x, food_y) || obstacle_exists(obstacles, food_x, food_y))
          generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
        type = (rand() > RAND_MAX / 2) ? Increase : Decrease;
        new_food = create_food(food_x, food_y, type);
        add_new_food(foods, new_food);
      }

      if (ch == 'p' || ch == 'P') {
        state = PAUSE;
        break;
      }

      if (ch == 'q' || ch == 'Q') {
        state = EXIT;
        break;
      }

      clear();
      draw_Gamewindow(window);
      mvprintw(20, 20, "Key Entered: %c", ch);
      mvprintw(21, 20, "Total Points: %d", score);
      mvprintw(22, 20, "Current Speed: %0.2f", speed);
      mvprintw(23, 20, "Speed-up Points: %d", temp_score);
      mvprintw(24, 20, "Lives: %d", lives);
      draw_Gamewindow(window);
      draw_snake(snake);
      draw_food(foods);
      draw_obstacle(obstacles);
      break;

    case DEAD:
      ch = get_char();

      clear();
      mvprintw(y_max / 2 - 1, x_max / 2 - 6, "YOU LOST!!!");
      mvprintw(y_max / 2, x_max / 2 - 8, "You scored: %d", score);
      mvprintw(y_max / 2 + 2, x_max / 2 - 9, "Press 'R' to restart");
      mvprintw(y_max / 2 + 3, x_max / 2 - 9, "Press 'Q' to quit");

      if (ch == 'q' || ch == 'Q') {
        save_score(score);
        clear();
        state = EXIT;
        break;
      }

      if (ch == 'r' || ch == 'R') {
        save_score(score);
        clear();
        is_start = false;
        has_dead = false;
        lives = 3;
        score = 0;
        speed = 1;
        temp_score = 0;
        is_dead = false;

        state = INIT;
        break;
      }
      break;

    // New State: PAUSE initialized when p is press
    case PAUSE:
      ch = get_char();

      // press p again to resume
      if (ch == 'p' || ch == 'P') {
        state = ALIVE;
        break;
      }

      // press q to quit the game
      if (ch == 'q' || ch == 'Q') {
        state = EXIT;
        break;
      }
    }
    refresh();
    nanosleep(&timeret, NULL);
  }
  endwin();
}
