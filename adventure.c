#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCATION_NAME_MAX_LENGTH 128
#define USER_COMMAND_MAX_LENGTH  1024

const char* start_location_name = "home";
const char* finish_location_name = "finish";

struct Map2D {
    int   width;
    int   height;
    char* data;
};

void set_map_2d(struct Map2D* map, int x, int y, char ch);

struct Map2D* new_map_2d(int width, int height) {
    struct Map2D* map = (struct Map2D*)malloc(sizeof(struct Map2D));
    map->width  = width;
    map->height = height;
    map->data   = (char*)malloc(sizeof(char) * width * height);
    for (int y = 0; y < map->height; y ++) {
        for (int x = 0; x < map->width; x ++) {
            int boolean = ((x * y / 16) % 8 == 0) ? 1 : 0;
            char ch = boolean ? 'o' : '.';
            set_map_2d(map, x, y, ch);
        }
    }
    return map;
}

struct Map2D* new_map_2d_from_initializer(char** map_initializer) {
    int width = 0;
    int height = 0;
    int y;
    for (y = 0; map_initializer[y] != NULL; y ++) {
         int len = strlen(map_initializer[y]);
	 if (width < len) {
	     width = len;
	 }
    }
    height = y;
    struct Map2D* map = new_map_2d(width, height);
    for (y = 0; y < height; y ++) {
        for (int x = 0; x < width; x ++) {
	    char ch = map_initializer[y][x];
	    set_map_2d(map, x, y, ch);
	}
    }
    return map;
}

void free_map_2d(struct Map2D* map) {
    free(map->data);
    free(map);
}

char get_map_2d(struct Map2D* map, int x, int y) {
    if (x < 0 ||
        y < 0 ||
        x >= map->width ||
        y >= map->height) {
        return '\0';
    } else {
        int index = y * map->width + x;
        return map->data[index];
    }
}

void set_map_2d(struct Map2D* map, int x, int y, char ch) {
    if (x < 0 ||
        y < 0 ||
        x >= map->width ||
        y >= map->height) {
     } else {
        int index = y * map->width + x;
        map->data[index] = ch;
    }
}

/*
index  |x = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
----------------------------------------
 y = 0 |    0  1  2  3  4  5  6  7  8  9
 y = 1 |   10 11 12 13 14 15 16 17 18 19
 y = 2 |   20 21 22 23 24 25 26 27 28 29
 y = 3 |   30 31 32 33 34 35 36 37 38 39
 y = 4 |   40 41 42 43 44 45 46 47 48 49
 .. .. |   .. .. .. .. .. .. .. .. .. ..
 .. .. |   .. .. .. .. .. .. .. .. .. ..
 .. .. |   .. .. .. .. .. .. .. .. .. ..


  width  = 10
  height = 10

  x | y | index
  0 | 0 | 0
  1 | 0 | 1
  2 | 0 | 2
  3 | 0 | 3
  o | 1 | 10
  

*/

void print_map_2d(struct Map2D* map) {
    for (int y = 0; y < map->height; y ++) {
        for (int x = 0; x < map->width; x ++) {
            char ch = get_map_2d(map, x, y);
            printf("%c", ch);
        }
        printf("\n");
    }
}

struct Adventure {
    char location_name[LOCATION_NAME_MAX_LENGTH];
    char user_command[USER_COMMAND_MAX_LENGTH];  
    double dollars;
    struct Map2D* map;
};

// example of function definition is passed a 1-dimensional array
int adventure_is_at_location(struct Adventure* self, const char* location_name) {
    return (strncmp(self->location_name, location_name, LOCATION_NAME_MAX_LENGTH) == 0);
}

// example of function definition is passed a 1-dimensional array
int adventure_user_command_equals(struct Adventure* self, const char* user_command) {
    return (strncmp(self->user_command, user_command, USER_COMMAND_MAX_LENGTH) == 0);
}

void adventure_print_location_description(struct Adventure* adventure) {
    print_map_2d(adventure->map);
    if (adventure_is_at_location(adventure, "home")) {
        printf("You are at home.  There is a street to the North.\n");
    } else if (adventure_is_at_location(adventure, "street")) {
        printf("You are in the street.  There is a house to the south.  There is a bank to the north.\n");
    } else if (adventure_is_at_location(adventure, "bank")) {
        printf("You are in a bank.\n");
    } else if (adventure_is_at_location(adventure, "finish")) {
        printf("You have finished the adventure!\n");
    } else {
        printf("You are at a location that you do not recognize.\n");
    }
}

void adventure_prompt_user_for_command(struct Adventure* adventure) {
    if (adventure_is_at_location(adventure, "bank")) {
        printf("How many dollars do you want?\n");
    } else {
        printf("What do you want to do?\n");
    }
    fgets(adventure->user_command, USER_COMMAND_MAX_LENGTH, stdin);
    int user_command_length = strlen(adventure->user_command);
    adventure->user_command[user_command_length - 1] = '\0';
}

// 2D array (char**)
char* default_map_initializer[] = {
    "................................",
    "................................",
    "................................",
    ".............*******............",
    "............*.......*...........",
    "............*.......&...........",
    "............*...................",
    "............*.......&...........",
    ".............*******............",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    NULL
};

struct Adventure* new_adventure(char* location_name, double dollars, char** map_initializer) {
    struct Adventure* adventure = (struct Adventure*)malloc(sizeof(struct Adventure));
    strcpy(adventure->location_name, location_name);
    strcpy(adventure->user_command, "");
    adventure->dollars = dollars;
    adventure->map = new_map_2d_from_initializer(map_initializer);
    return adventure;
}

void free_adventure(struct Adventure* adventure) {
    free_map_2d(adventure->map);
    free(adventure);
}

int adventure_is_done(struct Adventure* adventure) {
    if (adventure_is_at_location(adventure, finish_location_name)) {
        return 1; // true
    }
    return 0; // false
}

void adventure_execute_user_command(struct Adventure* adventure) {
    if (adventure_is_at_location(adventure, "bank")) {
        double dollars;
        if (sscanf(adventure->user_command, "%lg", &dollars) != 1) {
            printf("I do not know that number.\n");
        } else {
            printf("Ok, withdrawing %g dollars from your account.\n", dollars);
            adventure->dollars += dollars;
        }
        printf("You are now carrying %g dollars.\n", adventure->dollars);
        strcpy(adventure->location_name, "street");
    } else if (adventure_user_command_equals(adventure, "north") ||
               adventure_user_command_equals(adventure, "go north")) {
        printf("Going North!\n");
        if (adventure_is_at_location(adventure, "home")) {
            strcpy(adventure->location_name, "street");
        } else if (adventure_is_at_location(adventure, "street")) {
            strcpy(adventure->location_name, "bank");
        } else {
            printf("You cannot go north from here.\n");
        }
    } else if (adventure_user_command_equals(adventure, "south") ||
               adventure_user_command_equals(adventure, "go south")) {
        printf("Going South!\n");
        if (adventure_is_at_location(adventure, "street")) {
            strcpy(adventure->location_name, "home");
        } else if (adventure_is_at_location(adventure, "bank")) {
            strcpy(adventure->location_name, "street");
        } else {
            printf("You cannot go south from here.\n");
        }
    } else if (adventure_user_command_equals(adventure, "quit")) {
        printf("Ok, quitting the adventure.  Bye bye.\n");
        strcpy(adventure->location_name, finish_location_name);
    } else {
        printf("I do not understand your command.\n");
    }
}

int main(int argc, char** argv) {
    printf("This is an example adventure app.\n");
    printf("sizeof(struct Adventure) = %lu bytes\n", sizeof(struct Adventure));
    printf("sizeof(int) = %lu bytes\n", sizeof(int));
    printf("sizeof(double) = %lu bytes\n", sizeof(double));
    printf("sizeof(struct Map2D) = %lu bytes\n", sizeof(struct Map2D));
    printf("sizeof(void*) = %lu bytes\n", sizeof(void*));
    double dollars = 0;
    struct Adventure* adventure = new_adventure((char*)start_location_name, dollars, default_map_initializer);
    while (! adventure_is_done(adventure)) {
        adventure_print_location_description(adventure);
        adventure_prompt_user_for_command(adventure);
        adventure_execute_user_command(adventure);
    }
    adventure_print_location_description(adventure);
    free_adventure(adventure);
    return 0; // success
}
