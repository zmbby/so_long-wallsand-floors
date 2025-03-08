#include "so_long.h"

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

int render_next_frame(void *param) {
    t_data *data = (t_data *)param;
    data->loop_counter++;
    /*printf("Rendering frame %d\n", data->loop_counter); // Debug statement*/

    if (data->loop_counter % 6000 == 0) { // Adjust modulo for smoother animation
        update_position(data);
        mlx_clear_window(data->mlx, data->win); // Clear the window
        draw_map(data); // Redraw the walls
        data->loop_counter = 0;
        void **frames = (data->state == 0) ? data->idle_frames[data->direction] : data->running_frames[data->direction]; //hadi khasha tbadel l if o else .............................
        mlx_put_image_to_window(data->mlx, data->win, frames[data->current_frame], data->x, data->y);
        data->current_frame = (data->current_frame + 1) % 8;
    }
    return (0);
}

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

int handle_key_release(int keycode, t_data *data) {
    if (keycode >= KEY_LEFT && keycode <= KEY_DOWN) // Arrow keys
        data->state = 0;
    return (0);
}

int handle_exit(t_data *data) {
    //for (int i = 0;  i > )
    //mlx_destroy_image(void *mlx_ptr, void *img_ptr)
    mlx_destroy_window(data->mlx, data->win);
    exit(0);
}
