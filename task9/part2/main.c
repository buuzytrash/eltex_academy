#include <dirent.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH 1024

typedef struct
{
  WINDOW* win;
  char path[MAX_PATH];
  int selected_item;
} Panel;

void make_panel(Panel* panel)
{
  wclear(panel->win);
  box(panel->win, 0, 0);

  DIR* dir;
  struct dirent* entry;

  dir = opendir(panel->path);
  if (!dir) {
    perror("diropen");
    return;
  };
  int y = 1;

  while ((entry = readdir(dir)) != NULL) {
    if (y > getmaxy(panel->win) - 2) {
      break;
    }
    mvwprintw(panel->win, y, 1, "%s %d", entry->d_name, entry->d_reclen);
    if (panel->selected_item == y - 1) {
      wattron(panel->win, A_REVERSE);
      mvwprintw(panel->win, y, 1, "%s %d", entry->d_name, entry->d_reclen);
      wattroff(panel->win, A_REVERSE);
    }

    y++;
  }

  closedir(dir);
  wrefresh(panel->win);
}

void navigate_panel(Panel* panel, int ch)
{
  switch (ch) {
    case KEY_UP:
      if (panel->selected_item > 0) {
        panel->selected_item--;
      }
      break;
    case KEY_DOWN:
      panel->selected_item++;
      break;
    case KEY_ENTER:
    case '\n': {
      DIR* dir = opendir(panel->path);
      if (!dir) {
        return;
      }
      struct dirent* entry;
      int item = 0;

      while ((entry = readdir(dir)) != NULL) {
        if (item == panel->selected_item) {
          char new_path[MAX_PATH];
          snprintf(new_path, MAX_PATH, "%s/%s", panel->path, entry->d_name);

          struct stat st;
          if (stat(new_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            strncpy(panel->path, new_path, MAX_PATH);
            panel->selected_item = 0;
          }
          break;
        }
        item++;
      }

      closedir(dir);
      break;
    }
  }
  make_panel(panel);
}

void annihilate_win(WINDOW* win)
{
  wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(win);
  delwin(win);
}

int main()
{
  int rows, cols;
  int ch;

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  cbreak();

  getmaxyx(stdscr, rows, cols);
  printw("Press F1 to close");
  refresh();

  Panel panel_left = {
      .win = newwin(rows - 1, cols / 2, 1, 0), .path = ".", .selected_item = 0};
  Panel panel_right = {.win = newwin(rows - 1, cols / 2, 1, cols / 2),
                       .path = ".",
                       .selected_item = 0};

  make_panel(&panel_left);
  make_panel(&panel_right);

  int active_panel = 0;

  while ((ch = getch()) != KEY_F(1)) {
    if (ch == '\t') {
      active_panel = !active_panel;
    }
    else {
      if (active_panel == 0) {
        navigate_panel(&panel_left, ch);
      }
      else {
        navigate_panel(&panel_right, ch);
      }
    }
  }

  annihilate_win(panel_left.win);
  annihilate_win(panel_right.win);

  endwin();
  return 0;
}