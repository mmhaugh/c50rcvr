#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#define NOTINITIALIZED 0
#define INITIALIZED 1
#define ONCE 1
#define TWOARGS 2
#define SIZEOFBLOCK 512

// Defining type
typedef uint8_t BYTE;

// Prototype
int recursiveJpegWrite(FILE *inptr, FILE *img, int initializer);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != TWOARGS)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
	
    // Variable to determine filenames
    int blockCount = 0, temp = 0, temporary = 0;
    FILE *img;
    // Runs until recursive function called enough times to reach EOF of infile
    do
    {
        // Pointer to filename stored in ###.jpg format
        // The ### determined by blockCount variable
        char filename[8];
        sprintf(filename, "%03i.jpg", blockCount);

        // Pointer to append to new jpeg file
        img = fopen(filename, "a");
        if (img == NULL)
        {
            fprintf(stderr, "Could not open %s.\n", filename);
            return 2;
        }

	    else;
	    {
	        // Recursive function to write new jpeg files (returns 4 when new jpeg start bytes found)
            temporary = recursiveJpegWrite(inptr, img, NOTINITIALIZED);

            switch(temporary)
            {
                case 3:
                    fprintf(stderr, "Error in fseek function for %s.\n", argv[1]);
                    return 3;
                    break;

                case 2:
                    // Break loop
                    temp = temporary;
                    break;

                case 4:
                    // New jpeg found
                    blockCount++;
            }
        }
        if (img)
            fclose(img);
    }
    while(temp != 2);

    fclose(inptr);
    return 0;
}

// Recursively tests for jpeg identifying BYTEs and writes to jpeg if applicable
int recursiveJpegWrite(FILE *inptr, FILE *img, int initializer)
{
    // Temp storage of infile bytes
    BYTE buffer[512];

    // From infile reads size of memory block to buffer checking for EOF
    if (fread(buffer, SIZEOFBLOCK, ONCE, inptr) > 0)
    {
        // Test block for jpeg identifying BYTEs
        bool startingJpeg = (    buffer[0] == 0xff &&
                                 buffer[1] == 0xd8 &&
                                 buffer[2] == 0xff &&
                                (buffer[3] & 0xf0) == 0xe0    );

        switch(initializer)
        {
            // Not Initialized (initializer 0)
            case 0:

                // Jpeg starting BYTEs not confirmed calls itself (not initialized)
                if (startingJpeg == false)
                    return recursiveJpegWrite(inptr, img, NOTINITIALIZED);

                // Confirmed jpeg starting BYTEs writes to jpg file, calls itself (initialized)
                else;
                {
                    fwrite(buffer, sizeof(BYTE), SIZEOFBLOCK, img);
                    return recursiveJpegWrite(inptr, img, INITIALIZED);
                }
                break;

            // Initialized (initializer 1)
            case 1:
                // If jpeg start block conditions met
                if (startingJpeg == true)
                {
                    // Shift pointer back to start of block
                    if(fseek(inptr, -SIZEOFBLOCK, SEEK_CUR) != 0)
                        return 3;

                    else;
                        return 4;
                }
                // Write buffer to current image pointer, calls itself (initialized)
                else;
                {
                    fwrite(buffer, sizeof(BYTE), SIZEOFBLOCK, img);
                    return recursiveJpegWrite(inptr, img, INITIALIZED);
                }
                break;
        }
    }
    // Reached EOF and returns value to break while loop in main
    else;
        return 2;
}
