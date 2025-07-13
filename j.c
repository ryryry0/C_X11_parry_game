#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include <stdlib.h>

// 使用方法の表示
void print_usage(const char *progname) {
    printf("Usage: %s image.jpg <pixel_size> <rows> <cols> <output.txt>\n", progname);
    printf("Example: %s fireball.jpg 1 16 16 fireball.txt\n", progname);
}

int main(int argc, char *argv[])
{
    if (argc != 6) {
        print_usage(argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int pixel_size = atoi(argv[2]);
    int rows = atoi(argv[3]); // 縦のドット数
    int cols = atoi(argv[4]); // 横のドット数
    const char *output_filename = argv[5];

    int width, height, channels;
    unsigned char *img = stbi_load(filename, &width, &height, &channels, 3); // RGB固定

    if (img == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", filename);
        return 1;
    }

    if (width != cols * pixel_size || height != rows * pixel_size) {
        fprintf(stderr, "Error: Image size mismatch.\n");
        fprintf(stderr, "Expected: %dx%d, Got: %dx%d\n",
                cols * pixel_size, rows * pixel_size, width, height);
        stbi_image_free(img);
        return 1;
    }

    FILE *fp = fopen(output_filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open output file: %s\n", output_filename);
        stbi_image_free(img);
        return 1;
    }

    fprintf(fp, "unsigned char image[%d][%d][3] = {\n", rows, cols);
    for (int y = 0; y < rows; y++) {
        fprintf(fp, "  {");
        for (int x = 0; x < cols; x++) {
            int cx = x * pixel_size + pixel_size / 2;
            int cy = y * pixel_size + pixel_size / 2;
            int idx = (cy * width + cx) * 3;

            fprintf(fp, "{%3d,%3d,%3d}", img[idx], img[idx + 1], img[idx + 2]);
            if (x < cols - 1) fprintf(fp, ", ");
        }
        fprintf(fp, "}%s\n", (y < rows - 1) ? "," : "");
    }
    fprintf(fp, "};\n");

    fclose(fp);
    stbi_image_free(img);
    printf("Output written to %s\n", output_filename);
    return 0;
}
