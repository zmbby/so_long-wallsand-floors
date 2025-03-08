#include "so_long.h"

int check_ber(char *map) {
    int i = strlen(map);
    int tmp = i - 4; // Adjust for ".ber" (4 characters)
    if (tmp < 0)
        return (1);
    int a = strcmp(map + tmp, ".ber");
    return (a);
}

int loop_x1(char **tab, int y, int x, int i) {
    while (tab[y][x] != '\n' && tab[y][x]) {
        if (tab[y][x] != '1') { // Compare with '1', not "1"
            while (i >= 0) 
            {
                printf("a");
                free(tab[i]); // Free each line
                tab[i] = NULL; // Reset the pointer to avoid double-free
                i--;
            }
            return (0);
        }
        x++;
    }
    // Add more validation logic here if needed
    return (1);
}

/*int check_valid(char *map) {*/
/*    int x = 0, y = 0, i = 0;*/
/*    int fd = open(map, O_RDONLY); // Use O_RDONLY instead of O_RDWR*/
/*    if (fd < 0)*/
/*        return (0); // Failed to open file*/
/**/
/*    char *line;*/
/*    char **tab = malloc(sizeof(char *) * 16); // Allocate for 16 lines*/
/*    if (!tab) {*/
/*        close(fd);*/
/*        return (0); // Allocation failed*/
/*    }*/
/**/
/*    while (true)*/
/*    {*/
/*      line = get_next_line(fd)*/
/*      if (line == NULL)*/
/*        break;*/
/*      tab[i++] = line;*/
      /*free(line); // Free the current line*/
/*    }*/
/*    close(fd);*/
/**/
/*    // Validate the map (e.g., check walls, etc.)*/
/*    int valid = loop_x1(tab, y, x, i);*/
/**/
/*    // Free allocated memory*/
/*    for (int j = 0; j < i; j++) {*/
/*        free(tab[j]);*/
/*        tab[j] = NULL; // Reset the pointer to avoid double-free*/
/*    }*/
/*    free(tab);*/
/*    tab = NULL; // Reset the pointer to avoid double-free*/
/**/
/*    return (valid);*/
/*}*/

int check_map(char *map) {
    if (check_ber(map)) // Check if the file has a .ber extension
        return (0);

    int fd = open(map, O_RDONLY);
    if (fd < 0)
        return (0); // Failed to open file

    char *line;
    char **tab = malloc(sizeof(char *) * 16); // Allocate for 16 lines (adjust as needed)
    if (!tab) {
        close(fd);
        return (0); // Allocation failed
    }

    int i = 0;
    while ((line = get_next_line(fd))) {
        /*if (i == 15) { // Max 15 lines (adjust as needed)
            free(line);
            break;
        }*/
        tab[i++] = line;
    }
    close(fd);

    // Check if the map is surrounded by '1's
    int valid = 1;
    for (int y = 0; y < i; y++) {
        int len = strlen(tab[y]);
        // Remove the newline character if present
        if (len > 0 && tab[y][len - 1] == '\n')
            tab[y][len - 1] = '\0';

        for (int x = 0; tab[y][x]; x++) {
            // Check first and last rows
            if (y == 0 || y == i - 1) {
                if (tab[y][x] != '1') {
                    valid = 0;
                    break;
                }
            }
            // Check first and last characters of each row
            else if (x == 0 || tab[y][x + 1] == '\0') {
                if (tab[y][x] != '1') {
                    valid = 0;
                    break;
                }
            }
        }
        if (!valid)
            break;
    }

    // Free allocated memory
    for (int j = 0; j < i; j++) {
        free(tab[j]);
        tab[j] = NULL;
    }
    free(tab);
    tab = NULL;

    return (valid);
}

char **read_map(char *map_file) {
    int fd = open(map_file, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open map file: %s\n", map_file);
        exit(1);
    }

    char *line;
    char **map = malloc(sizeof(char *) * 16); // Adjust size as needed
    if (!map) {
        close(fd);
        printf("Memory allocation failed\n");
        exit(1);
    }

    int i = 0;
    while ((line = get_next_line(fd))) {
        map[i++] = line;
    }
    map[i] = NULL; // Null-terminate the array

    close(fd);
    return map;
}
