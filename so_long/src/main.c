#include "so_long.h"

int main(int ac, char **av) {
    if (ac != 2) { // Ensure exactly one argument (the map file) is provided
        printf("Usage: %s <map_file>\n", av[0]);
        return (1);
    }
    /*char *map1 = av[1]; // The map file is the first argument*/
    /*int check = check_map(map1);*/
    /*if (!check) {*/
    /*    printf("Invalid map file.\n");*/
    /*    return (1);*/
    /*}*/

    t_data data;
    data.map = read_map(av[1]);
    if (!data.map)
        exit(1);
    int map_width = strlen(data.map[0]); // Number of columns
    int map_height = 0;
    while (data.map[map_height]) map_height++;

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

    /*data = (t_data){0}; // had zebi howa seg fault salina hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
    data.x = 64;
    data.y = 64;*/
    data.current_frame = 0;
    data.loop_counter = 0;
    data.direction = 0; // Start facing down
    data.state = 0; // Start in idle state
    data.x = 64;
    data.y = 64;
    data.wall_img = load_image(data.mlx, "wall_floor/wall.xpm"); // Load the wall image
    
    // Read the map

    // Draw the map
    /*mlx_clear_window(data.mlx, data.win);*/
    /*draw_map(&data, map);*/

    // Free the map
    /*for (int i = 0; data.map[i]; i++)*/
    /*    free(data.map[i]);*/
    /*free(data.map);*/

    // Set up the rendering loop
    

    // Set up event hooks
    mlx_hook(data.win, KeyPress, KeyPressMask,handle_key_press, &data);
    mlx_hook(data.win, KeyRelease, KeyReleaseMask, handle_key_release, &data);
    mlx_hook(data.win, DestroyNotify, NoEventMask, handle_exit, &data); // Window close
    mlx_loop_hook(data.mlx, render_next_frame, &data);
    // Start the MiniLibX loop
    mlx_loop(data.mlx);

    return (0);
}
