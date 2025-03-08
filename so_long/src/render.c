#include "so_long.h"

void *load_image(void *mlx, char *path) {
    int width, height;
    void *img = mlx_xpm_file_to_image(mlx, path, &width, &height);
    if (!img) {
        printf("Failed to load image: %s\n", path);
        exit(1);
    }
    return img;
}


void draw_map(t_data *data) {
    int tile_size = 50; // Each tile is 50x50 pixels

  /*for (int i = 0; data->map[i]; i++)*/
  /*  printf("%s", data->map[i]);*/
  /*exit(1);*/
    for (int y = 0; data->map[y]; y++) {
        for (int x = 0; data->map[y][x]; x++) {
            if (data->map[y][x] == '1') {
    /*            // Calculate the position to draw the wall*/
                int screen_x = x * tile_size;
                int screen_y = y * tile_size;
    /*            */
                /*// Draw the wall tile*/
                mlx_put_image_to_window(data->mlx, data->win, data->wall_img, screen_x, screen_y);
                /*//printf("Wall drawn at (%d, %d)\n", screen_x, screen_y); // Debug statement*/
            }
        }
    }
}
