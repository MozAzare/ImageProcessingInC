/* This coursework specification, and the example code provided during the
 * course, is Copyright 2024 Heriot-Watt University.
 * Distributing this coursework specification or your solution to it outside
 * the university is academic misconduct and a violation of copyright law. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};

/* An image loaded from a file. */
struct Image {
    /* TODO: Question 1 */
    char header[4];
    int width; //number of columns
    int height; //number of rows
    struct Pixel *pixels; // Pointer to an array of struct Pixel
};

/* Free a struct Image */
void free_image(struct Image *img)
{
    /* TODO: Question 2a */
    // First, check if the img pointer is not NULL
    if (img != NULL){
        // Free the dynamically allocated pixel data
        if (img->pixels != NULL) {
            free(img->pixels);
            img->pixels = NULL; // Set the pointer to NULL after freeing
        }
    }
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename) {
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Attempt to read the HSHEX header */
    char format[6]; // Buffer to hold the format string
    if (fscanf(f, "%5s", format) != 1 || strcmp(format, "HSHEX") != 0) {
        fprintf(stderr, "Invalid format or unable to read format from %s.\n", filename);
        fclose(f);
        return NULL;
    }

    /* Allocate the Image object */
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL) {
        fprintf(stderr, "Memory allocation for image failed.\n");
        fclose(f);
        return NULL;
    }

    /* Read image dimensions */
    if (fscanf(f, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Failed to read image dimensions from %s.\n", filename);
        free(img);
        fclose(f);
        return NULL;
    }

    /* Allocate memory for the pixels array based on the read dimensions */
    img->pixels = malloc(sizeof(struct Pixel) * img->width * img->height);
    if (img->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel data.\n");
        free(img); // Don't forget to free the Image structure if pixel allocation fails
        fclose(f);
        return NULL;
    }

    /* Read the pixel data */
    for (int i = 0; i < img->width * img->height; i++) {
        unsigned int red, green, blue;
        if (fscanf(f, "%4x%4x%4x", &red, &green, &blue) != 3) {
            fprintf(stderr, "Failed to read pixel data at pixel %d in %s.\n", i, filename);
            free_image(img); // Utilize the previously defined free_image function to clean up
            fclose(f);
            return NULL;
        }
        img->pixels[i].red = (unsigned short)red;
        img->pixels[i].green = (unsigned short)green;
        img->pixels[i].blue = (unsigned short)blue;
    }

    /* Close the file */
    fclose(f);

    return img;
}


/* Write img to file filename. Return true on success, false on error. */
bool save_image(const struct Image *img, const char *filename)
{
    /* TODO: Question 2c */

    // Open the file for writing
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Could not open file %s for writing.\n", filename);
        return false;
    }

    // The HSHEX header
    fprintf(f, "HSHEX\n%d %d\n", img->width, img->height);

    // The pixel data : Each pixel's RGB components are written in hexadecimal format with %04X to ensure a 4-digit hexadecimal output, preserving leading zeros. Spaces are added between RGB values for readability, and a newline character is inserted at the end of each row of pixels.
    for (int i = 0; i < img->width * img->height; i++) {
        fprintf(f, "%04X %04X %04X ", img->pixels[i].red, img->pixels[i].green, img->pixels[i].blue);
        if ((i + 1) % img->width == 0) { // New line at the end of each row
            fprintf(f, "\n");
        }
    }

    // Close the file
    fclose(f);
    return true;
}

/* Allocate a new struct Image and copy an existing struct Image's contents
 * into it. On error, returns NULL. */
struct Image *copy_image(const struct Image *source)
{
    if (source == NULL) {
        return NULL; // Cannot copy from a NULL source
    }

    // Allocate memory for the new Image object
    struct Image *copy = malloc(sizeof(struct Image));
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed for image copy.\n");
        return NULL;
    }

    // Copy image dimensions
    copy->width = source->width;
    copy->height = source->height;

    // Allocate memory for the pixel data
    copy->pixels = malloc(copy->width * copy->height * sizeof(struct Pixel));
    if (copy->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel data.\n");
        free(copy); // Free the Image object if pixel allocation fails
        return NULL;
    }

    // Copy the pixel data
    for (int i = 0; i < copy->width * copy->height; i++) {
        copy->pixels[i] = source->pixels[i];
    }

    return copy;
}

/* Perform your first task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns a new struct Image containing the result, or NULL on error. */
struct Image *apply_NOISE(const struct Image *source, int noise_strength)
{
    /* TODO: Question 3 */
    if (source == NULL) {
        return NULL; // Handle null source pointer
    }

    struct Image *noisy_image = copy_image(source); // copy of the original image to avoid modifying it
    if (noisy_image == NULL) {
        fprintf(stderr, "Failed to create a copy of the image.\n");
        return NULL;
    }

    for (int i = 0; i < noisy_image->width * noisy_image->height; i++) {
        // Apply noise to each color component of each pixel
        int noise;
        for (int color = 0; color < 3; color++) {
            //generate random noise
            noise = (rand() % (2 * noise_strength + 1)) - noise_strength;
            // Apply the result for each color component
            unsigned short* color_component;
            // We decide which color component we're working with (red, green, or blue) based on the loop's current iteration, and we get a pointer to it.
            if (color == 0) {
                color_component = &noisy_image->pixels[i].red;
            } else if (color == 1) {
                color_component = &noisy_image->pixels[i].green;
            } else {
                color_component = &noisy_image->pixels[i].blue;
            }
            
            //add the noise value to the current color component's value.
            int noisy_value = (int)(*color_component) + noise;

            // prevent the color values from wrapping around if they get too high or too low.
            if (noisy_value < 0) {
                *color_component = 0;
            } else if (noisy_value > 65535) {
                *color_component = 65535;
            } else {
                *color_component = (unsigned short)noisy_value;
            }
        }
    }

    return noisy_image;
}

bool apply_HIST(const struct Image *source)
{
    /* TODO: Question 4 */
    // Array to hold the count of each color value (0-255)
    unsigned int histogram[256] = {0};

    // Count occurrences of each value for all color components
    for (int i = 0; i < source->width * source->height; i++) {
        struct Pixel pixel = source->pixels[i];
        histogram[pixel.red >> 8]++;   // Shift right to fit into 8-bit range
        histogram[pixel.green >> 8]++; 
        histogram[pixel.blue >> 8]++;
    }

    // Print the histogram
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > 0) { // Optional: only print values that occur
            printf("Value %d: %d pixels\n", i, histogram[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* TODO: Remove this if you don't need to use rand() */
    
    /* Check command-line arguments */

    if (argc != 4) {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    /* Load the input image */
    struct Image *in_img = load_image(argv[1]);
    if (in_img == NULL) {
        return 1;
    }

    /* Apply the first process */
    int noise_strength = atoi(argv[3]); // Convert the noise_strength from string to integer using atoi
    struct Image *out_img = apply_NOISE(in_img,noise_strength);
    if (out_img == NULL) {
        fprintf(stderr, "Noise process failed.\n");
        free_image(in_img);
        return 1;
    }

    /* Apply the second process */
    apply_HIST(out_img);
    // if (!apply_HIST(out_img)) {
    //     fprintf(stderr, "Histogram process failed.\n");
    //     free_image(in_img);
    //     free_image(out_img);
    //     return 1;
    // }

    /* Save the output image */
    if (!save_image(out_img, argv[2])) {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    free_image(in_img);
    free_image(out_img);
    return 0;

    // if (argc != 3) {
    //     fprintf(stderr, "Usage: %s inputfile outputfile\n", argv[0]);
    //     return 1;
    // }

    // struct Image *img = load_image(argv[1]);
    // if (img == NULL) {
    //     return 1; // load_image will handle error messages
    // }

    // if (!save_image(img, argv[2])) {
    //     fprintf(stderr, "Failed to save image.\n");
    //     free_image(img);
    //     return 1;
    // }

    // free_image(img); // Cleanup
    // printf("Image saved successfully.\n");
    // return 0;
}
