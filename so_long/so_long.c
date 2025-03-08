/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 21:25:47 by zoentifi          #+#    #+#             */
/*   Updated: 2025/01/12 19:39:40 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Function to update the player's position
    t_data data;

void update_position(t_data *data) {
    if (data->state == 0)
        return;
    if (data->direction == 0 && data->y + SPEED + 130 < data->win_height)
        data->y += SPEED;
    if (data->direction == 1 && data->x - SPEED > 0)
        data->x -= SPEED;
    if (data->direction == 2 && data->x + SPEED + 64 < data->win_width)
        data->x += SPEED;
    if (data->direction == 3 && data->y - SPEED > 0)
        data->y -= SPEED;
}

// Function to render the next frame
int render_next_frame(void *param) {
    t_data *data = (t_data *)param;
    data->loop_counter++;
    if (data->loop_counter % 6000 == 0) { // Adjust modulo for smoother animation
        update_position(data);
        mlx_clear_window(data->mlx, data->win);
        void **frames = (data->state == 0) ? data->idle_frames[data->direction] : data->running_frames[data->direction];
        mlx_put_image_to_window(data->mlx, data->win, frames[data->current_frame], data->x, data->y);
        data->current_frame = (data->current_frame + 1) % 8;
    }
    return (0);
}

// Function to handle key press events
int handle_key_press(int keycode, t_data *data) {
    if (keycode == KEY_ESC) { // Escape key to exit
        mlx_destroy_window(data->mlx, data->win);
        exit(0);
    } else if (keycode == KEY_LEFT) { // Left arrow key
        data->direction = 1;
        data->state = 1; // Running
    } else if (keycode == KEY_RIGHT) { // Right arrow key
        data->direction = 2;
        data->state = 1; // Running
    } else if (keycode == KEY_UP) { // Up arrow key
        data->direction = 3;
        data->state = 1; // Running
    } else if (keycode == KEY_DOWN) { // Down arrow key
        data->direction = 0;
        data->state = 1; // Running
    }
    return (0);
}

// Function to handle key release events
int handle_key_release(int keycode, t_data *data) {
    if (keycode >= KEY_LEFT && keycode <= KEY_DOWN) // Arrow keys
        data->state = 0;
    return (0);
}

// Function to handle window close event
int handle_exit(t_data *data) {
    mlx_destroy_window(data->mlx, data->win);
    exit(0);
}

// Function to check if the map file has a .ber extension
int check_ber(char *map) {
    int i = strlen(map);
    int tmp = i - 4; // Adjust for ".ber" (4 characters)
    if (tmp < 0)
        return (1);
    int a = strcmp(map + tmp, ".ber");
    return (a);
}

// Function to validate the map file
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



int check_valid(char *map) {
    int x = 0, y = 0, i = 0;
    int fd = open(map, O_RDONLY); // Use O_RDONLY instead of O_RDWR
    if (fd < 0)
        return (0); // Failed to open file

    char *line;
    char **tab = malloc(sizeof(char *) * 16); // Allocate for 16 lines
    if (!tab) {
        close(fd);
        return (0); // Allocation failed
    }

    while ((line = get_next_line(fd))) { // Fix missing parenthesis
        if (i == 15) { // Max 15 lines
            free(line); // Free the current line
            break;
        }
        tab[i++] = line;
    }
    close(fd);

    // Validate the map (e.g., check walls, etc.)
    int valid = loop_x1(tab, y, x, i);

    // Free allocated memory
    for (int j = 0; j < i; j++) {
        free(tab[j]);
        tab[j] = NULL; // Reset the pointer to avoid double-free
    }
    free(tab);
    tab = NULL; // Reset the pointer to avoid double-free

    return (valid);
}

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
        /*if (i == 15) { // Adjust max lines as needed
            free(line);
            break;
        }*/
        map[i++] = line;
    }
    map[i] = NULL; // Null-terminate the array

    close(fd);
    return map;
}

void draw_map(t_data *data, char **map) {
    int tile_size = 50; // Each tile is 50x50 pixels
    void *wall_img = load_image(data->mlx, "wall_floor/wall.xpm"); // Load the wall image

    for (int y = 0; map[y]; y++) {
        for (int x = 0; map[y][x]; x++) {
            if (map[y][x] == '1') {
                // Calculate the position to draw the wall
                int screen_x = x * tile_size;
                int screen_y = y * tile_size;


                // Draw the wall tile
                mlx_put_image_to_window(data->mlx, data->win, wall_img, screen_x, screen_y);
                printf("Wall drawn at (%d, %d)\n", screen_x, screen_y);
                
            }
        }
    }
}

void *load_image(void *mlx, char *path) {
    int width, height;
    void *img = mlx_xpm_file_to_image(mlx, path, &width, &height);
    if (!img) {
        printf("Failed to load image: %s\n", path);
        exit(1);
    }
    return img;
}

int main(int ac, char **av) {
    if (ac != 2) { // Ensure exactly one argument (the map file) is provided
        printf("Usage: %s <map_file>\n", av[0]);
        return (1);
    }
    char *map1 = av[1]; // The map file is the first argument
    /*int check = check_map(map1);*/
    /*if (!check) {*/
    /*    printf("Invalid map file.\n");*/
    /*    return (1);*/
    /*}*/

    t_data data;
    char **map = read_map(av[1]);
    int map_width = strlen(map[0]); // Number of columns
    int map_height = 0;
    while (map[map_height]) map_height++;

    data.win_width = map_width * 50; // Each tile is 50x50 pixels
    data.win_height = map_height * 50;

    int width, height;

    // Paths for idle and running frames
    char *idle_paths[4][8] = {
        { // Down idle
            "idle/down/down_1.xpm", "idle/down/down_2.xpm", "idle/down/down_3.xpm",
            "idle/down/down_4.xpm", "idle/down/down_5.xpm", "idle/down/down_6.xpm",
            "idle/down/down_7.xpm", "idle/down/down_8.xpm"
        },
        { // Left idle
            "idle/left/left_1.xpm", "idle/left/left_2.xpm", "idle/left/left_3.xpm",
            "idle/left/left_4.xpm", "idle/left/left_5.xpm", "idle/left/left_6.xpm",
            "idle/left/left_7.xpm", "idle/left/left_8.xpm"
        },
        { // Right idle
            "idle/right/right_1.xpm", "idle/right/right_2.xpm", "idle/right/right_3.xpm",
            "idle/right/right_4.xpm", "idle/right/right_5.xpm", "idle/right/right_6.xpm",
            "idle/right/right_7.xpm", "idle/right/right_8.xpm"
        },
        { // Up idle
            "idle/up/up_1.xpm", "idle/up/up_2.xpm", "idle/up/up_3.xpm",
            "idle/up/up_4.xpm", "idle/up/up_5.xpm", "idle/up/up_6.xpm",
            "idle/up/up_7.xpm", "idle/up/up_8.xpm"
        }
    };
    char *running_paths[4][8] = {
        { // Down running
            "running/down/down_1.xpm", "running/down/down_2.xpm", "running/down/down_3.xpm",
            "running/down/down_4.xpm", "running/down/down_5.xpm", "running/down/down_6.xpm",
            "running/down/down_7.xpm", "running/down/down_8.xpm"
        },
        { // Left running
            "running/left/left_1.xpm", "running/left/left_2.xpm", "running/left/left_3.xpm",
            "running/left/left_4.xpm", "running/left/left_5.xpm", "running/left/left_6.xpm",
            "running/left/left_7.xpm", "running/left/left_8.xpm"
        },
        { // Right running
            "running/right/right_1.xpm", "running/right/right_2.xpm", "running/right/right_3.xpm",
            "running/right/right_4.xpm", "running/right/right_5.xpm", "running/right/right_6.xpm",
            "running/right/right_7.xpm", "running/right/right_8.xpm"
        },
        { // Up running
            "running/up/up_1.xpm", "running/up/up_2.xpm", "running/up/up_3.xpm",
            "running/up/up_4.xpm", "running/up/up_5.xpm", "running/up/up_6.xpm",
            "running/up/up_7.xpm", "running/up/up_8.xpm"
        }
    };

    data.mlx = mlx_init();
    if (!data.mlx)
        return (printf("Failed to initialize MiniLibX\n"), 1);
    
    data.win = mlx_new_window(data.mlx, data.win_width, data.win_height, "So Long");
    if (!data.win)
        return (printf("Failed to create window\n"), 1);

    for (int dir = 0; dir < 4; dir++) {
        for (int i = 0; i < 8; i++) {
            data.idle_frames[dir][i] = mlx_xpm_file_to_image(data.mlx, idle_paths[dir][i], &width, &height);
            if (!data.idle_frames[dir][i])
                return (printf("Failed to load idle image: %s\n", idle_paths[dir][i]), 1);

            data.running_frames[dir][i] = mlx_xpm_file_to_image(data.mlx, running_paths[dir][i], &width, &height);
            if (!data.running_frames[dir][i])
                return (printf("Failed to load running image: %s\n", running_paths[dir][i]), 1);
        }
    }

    data.current_frame = 0;
    data.loop_counter = 0;
    data.direction = 0; // Start facing down
    data.state = 0; // Start in idle state
    data.x = 64;
    data.y = 64;
    
    // Read the map
    

    // Draw the map
    mlx_clear_window(data.mlx, data.win);
    draw_map(&data, map);

    // Free the map
    for (int i = 0; map[i]; i++)
        free(map[i]);
    free(map);

    mlx_loop_hook(data.mlx, render_next_frame, &data);
    mlx_hook(data.win, KeyPress, KeyPressMask, handle_key_press, &data);
    mlx_hook(data.win, KeyRelease, KeyReleaseMask, handle_key_release, &data);
    mlx_hook(data.win, 17, 0, handle_exit, &data); // Window close

    mlx_loop(data.mlx);

    return (0);
}
