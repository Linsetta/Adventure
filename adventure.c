#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCATION_NAME_MAX_LENGTH 128
#define USER_COMMAND_MAX_LENGTH  1024

typedef int bool;

const bool false = (bool)0;
const bool true  = (bool)(!0);

const char* start_location_name = "home";
const char* finish_location_name = "finish";

// Map2D BEGIN

typedef struct Map2D_s {
    int   width;
    int   height;
    char* data;
} Map2D;

void set_map_2d(Map2D* map, int x, int y, char ch);

Map2D* new_map_2d(int width, int height) {
    Map2D* map = (Map2D*)malloc(sizeof(Map2D));
    map->width  = width;
    map->height = height;
    map->data   = (char*)malloc(sizeof(char) * width * height);
    for (int y = 0; y < map->height; y ++) {
        for (int x = 0; x < map->width; x ++) {
            bool boolean = ((x * y / 16) % 8 == 0) ? true : false;
            char ch = boolean ? 'o' : '.';
            set_map_2d(map, x, y, ch);
        }
    }
    return map;
}

Map2D* new_map_2d_from_initializer(char** map_initializer) {
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
    Map2D* map = new_map_2d(width, height);
    for (y = 0; y < height; y ++) {
        for (int x = 0; x < width; x ++) {
	    char ch = map_initializer[y][x];
	    set_map_2d(map, x, y, ch);
	}
    }
    return map;
}

void free_map_2d(Map2D* map) {
    free(map->data);
    free(map);
}

char get_map_2d(Map2D* map, int x, int y) {
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

void set_map_2d(Map2D* map, int x, int y, char ch) {
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

void print_map_2d(Map2D* map) {
    for (int y = 0; y < map->height; y ++) {
        for (int x = 0; x < map->width; x ++) {
            char ch = get_map_2d(map, x, y);
            printf("%c", ch);
        }
        printf("\n");
    }
}

// Map2D END


// CommandHistory BEGIN

typedef struct CommandHistory_s {
    int    commands_length;
    char** commands;
} CommandHistory;

CommandHistory* new_command_history(int commands_length) {
    CommandHistory* command_history = (CommandHistory*)malloc(sizeof(CommandHistory));
    command_history->commands_length = commands_length;
    command_history->commands = (char**)malloc(sizeof(char*) * command_history->commands_length);
    // initialize all elements in (char**) array
    for (int i = 0; i < command_history->commands_length; i ++) {
        command_history->commands[i] = (char*)NULL; // initiailize single element in (char**) array
    }
    return command_history;
}

void free_command_history(CommandHistory* command_history) {
    for (int i = 0; i < command_history->commands_length; i ++) {
        if (command_history->commands[i] != NULL) {
	    free (command_history->commands[i]);
	}
    }
    free(command_history->commands);
    free(command_history);
}

void print_command_history(CommandHistory* command_history) {
    printf("Command history = [\n");
    for (int i = 0; i < command_history->commands_length; i ++) {
        if (command_history->commands[i] != NULL) {
	    printf("  %s\n", command_history->commands[i]);
	}
    }
    printf("]\n");
}

void command_history_add_command(CommandHistory* command_history, char* command) {
    int command_length = strlen(command);
    char* new_command = (char*)malloc(sizeof(char) * (command_length + 1));
    strcpy(new_command, command);
    // <-- new_command is initialized at this point
    char* last_command = command_history->commands[command_history->commands_length - 1];
    // free last_command string, if it exists
    if (last_command != NULL) {
        free(last_command);
    }
    // move all strings +1 in array of strings (char**)
    for (int i = command_history->commands_length - 2; i >= 0; i --) {
        command_history->commands[i + 1] = command_history->commands[i];
    }
    // add new_command to front of array of strings (char**)
    command_history->commands[0] = new_command;
}

// CommandHistory END


// Adventure BEGIN

typedef struct Adventure_s {
    char location_name[LOCATION_NAME_MAX_LENGTH];
    char user_command[USER_COMMAND_MAX_LENGTH];  
    double dollars;
    Map2D* map;
    CommandHistory* command_history;
} Adventure;

// example of function definition is passed a 1-dimensional array
bool adventure_is_at_location(Adventure* adventure, const char* location_name) {
    return (strncmp(adventure->location_name, location_name, LOCATION_NAME_MAX_LENGTH) == 0);
}

// example of function definition is passed a 1-dimensional array
bool adventure_user_command_equals(Adventure* adventure, const char* user_command) {
    return (strncmp(adventure->user_command, user_command, USER_COMMAND_MAX_LENGTH) == 0);
}

void adventure_print_location_description(Adventure* adventure) {
    print_map_2d(adventure->map);
    print_command_history(adventure->command_history);
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

void adventure_prompt_user_for_command(Adventure* adventure) {
    if (adventure_is_at_location(adventure, "bank")) {
        printf("How many dollars do you want?\n");
    } else {
        printf("What do you want to do?\n");
    }
    fgets(adventure->user_command, USER_COMMAND_MAX_LENGTH, stdin);
    int user_command_length = strlen(adventure->user_command);
    adventure->user_command[user_command_length - 1] = '\0';
    // <--- we have the user command here!
    command_history_add_command(adventure->command_history, adventure->user_command);
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

Adventure* new_adventure(char* location_name, double dollars, char** map_initializer, int commands_length) {
    Adventure* adventure = (Adventure*)malloc(sizeof(Adventure));
    strcpy(adventure->location_name, location_name);
    strcpy(adventure->user_command, "");
    adventure->dollars = dollars;
    adventure->map = new_map_2d_from_initializer(map_initializer);
    adventure->command_history = new_command_history(commands_length);
    return adventure;
}

void free_adventure(Adventure* adventure) {
    free_command_history(adventure->command_history);
    free_map_2d(adventure->map);
    free(adventure);
}

bool adventure_is_done(Adventure* adventure) {
    if (adventure_is_at_location(adventure, finish_location_name)) {
        return true;
    }
    return false;
}

void adventure_execute_user_command(Adventure* adventure) {
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

// Adventure END


int main(int argc, char** argv) {
    printf("This is an example adventure app.\n");
    printf("sizeof(Adventure) = %lu bytes\n", sizeof(Adventure));
    printf("sizeof(int) = %lu bytes\n", sizeof(int));
    printf("sizeof(double) = %lu bytes\n", sizeof(double));
    printf("sizeof(Map2D) = %lu bytes\n", sizeof(Map2D));
    printf("sizeof(void*) = %lu bytes\n", sizeof(void*));
    double dollars = 0;
    int commands_length = 10;
    Adventure* adventure = new_adventure((char*)start_location_name, dollars, default_map_initializer, commands_length);
    while (! adventure_is_done(adventure)) {
        adventure_print_location_description(adventure);
        adventure_prompt_user_for_command(adventure);
        adventure_execute_user_command(adventure);
    }
    adventure_print_location_description(adventure);
    free_adventure(adventure);
    return 0; // success
}
