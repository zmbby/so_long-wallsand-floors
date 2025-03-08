/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 21:36:12 by zoentifi          #+#    #+#             */
/*   Updated: 2025/01/05 21:36:15 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
#define SO_LONG_H

#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/X.h>       // For KeyPress, KeyRelease, etc.
#include <X11/keysym.h>  // For keycodes like XK_Escape, XK_Left, etc.
#include <fcntl.h>
#include <unistd.h>
#include "../get_next_line/get_next_line.h"

// Window dimensions and speed
#define SPEED 15

// Keycodes for Linux
#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define KEY_DOWN 65364
#define KEY_UP 65362

// Structure for game data
typedef struct s_data {
    void    *mlx;
    void    *win;
    void    *idle_frames[4][8]; // Idle animation frames
    void    *running_frames[4][8]; // Running animation frames
    int     current_frame;  // Index of the current frame
    int     loop_counter;  // Counter to control the frame rate
    int     direction;     // Current direction: 0 = down, 1 = left, 2 = right, 3 = up
    int     state;  // Current state: 0 = idle, 1 = running
    int     x;
    int     y;
    int     win_width; // Window width
    int     win_height; // Window height
    char    *wall_img;
    char    **map;
} t_data;

// Function prototypes
void update_position(t_data *data);
int render_next_frame(void *param);
int handle_key_press(int keycode, t_data *data);
int handle_key_release(int keycode, t_data *data);
int handle_exit(t_data *data);
int check_ber(char *map);
int loop_x1(char **tab, int y, int x, int i);
int check_valid(char *map);
int check_map(char *map);
char **read_map(char *map_file);
void draw_map(t_data *data);
void *load_image(void *mlx, char *path);

#endif
