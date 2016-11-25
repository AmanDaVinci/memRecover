/*
* 
* Memory Recovery
* Recovers deleted JPEGs from a corrupted raw image of a memory card
*
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int isJPG(char* buffer);

int main(int argc, char* argv[])
{
    if (argc != 1)
    {
        printf("No input required.\n");
        return 1;
    }
    
    FILE* infile = fopen("card.raw", "r");
    if (infile == NULL)
    {
        printf("Could not open.\n");
        return 2;
    }
    if(ferror(infile))
    {
        printf("Error Occured.\n");
    }
    
    // File serial number
    int serial = 0;
    char fname[8];
        
    while (!feof(infile))
    {
        // buffer storage
        char buffer[512];
        // reading a block at a time
        fread(&buffer, sizeof(buffer), 1, infile);
      
        // Check for JPG files
        if(isJPG(buffer))
        {
            // Naming the files
            if (serial < 10) { sprintf(fname, "00%i.jpg", serial); }
            else  { sprintf(fname, "0%i.jpg", serial); }
            FILE* outfile = fopen(fname, "w");
            
            // Start retrieving image
            do
            {
                fwrite(buffer, sizeof(buffer[0]), sizeof(buffer)/sizeof(buffer[0]), outfile);
                fread(&buffer, sizeof(buffer), 1, infile);
                if (feof(infile)) { break; } 
            }while(!isJPG(buffer));
            
            // Close file and increase serial number
            fclose(outfile);
            serial++;
            // Go back a block to get next image
            fseek(infile, -512, SEEK_CUR);
        }
    }
    
    fclose(infile);
    return 0;
}

int isJPG(char* buffer)
{
    if (buffer[0] == (char)255 && buffer[1] == (char)216 && buffer[2] == (char)255 && (buffer[3] == (char)224 || buffer[3] == (char)225) )
        return 1;
    else
        return 0;
}
