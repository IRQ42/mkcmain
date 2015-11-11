/****************************************************************************
 * mkcmain -- creates new main.c with empty main boilerplate template       *
 * *************************************************************************/

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * Prints an optional error message, and kills process.
 * At the end of the error message, a newline is printed.
 * If msg is a NULL pointer then nothing will be printed.
 */
void die(int status, const char *msg)
{
    // print error message 
    if (msg) {
        fprintf(stderr, "%s: %s\n", "mkcmain", msg);
    }

    exit(status);
}

int main(void)
{    
    const char *comment_text =
        "/***************************************************************************\n"
        " * Created by mkcmain                                                      *\n"
        " **************************************************************************/\n\n";
    size_t      comment_text_size;

    const char  *cmain_text = "int main(int argc, char *argv[])\n"
                              "{\n\n"
                              "\treturn 0;\n"
                              "}\n";
    size_t      cmain_text_size;

    const char  *filename   = "main.c";
    char        *cw_dir;
    char        *filepath;
    FILE        *file;
    struct stat sb;

    // attempt to get the current working dir
    errno  = 0;
    cw_dir = get_current_dir_name();
    if(NULL == cw_dir) {
        perror("get_current_dir_name()");
        die(EXIT_FAILURE, "could not get current directory name");
    }

    // allocate memory for path
    if (NULL == (filepath = malloc((strlen(cw_dir) + 1 + strlen(filename) + 1) * sizeof(char)))) {
        die(EXIT_FAILURE, "could not allocate memory for file path name"); } 
    // construct file path
    strcpy(filepath, cw_dir);
    strcat(filepath, "/");
    strcat(filepath, filename);
    free(cw_dir);

    // check to see if file exists 
    if (0 == stat(filepath, &sb)) {
        bool abort = false;

        // see if this is a regular file
        if (!(sb.st_mode & S_IFREG)) {
            abort = true;
            fprintf(stderr, "%s: %s is not a regular file\n", "mkcmain", filepath);
        } else {
            // prompt user to replace file
             
            char response;

            fprintf(stdout, "%s: %jd byte file %s exists. Replace file? [y/N?] ",
                    "mkcmain", (intmax_t) sb.st_size, filepath);
            response = fgetc(stdin);
            if (response != 'y' && response != 'Y') {
                abort = true;
            }
        }
    
        if (abort) {
            free(filepath);
            die(EXIT_FAILURE, "no file written");
        }
    }

    // try to open file for writing
    errno = 0;
    if (NULL == (file = fopen(filepath, "w"))) {
        perror("fopen");
        fprintf(stderr, "%s: could not open %s for writing\n", "mkcmain", filepath);
        free(filepath);
        die(EXIT_FAILURE, NULL);
    }
   
    // try writing comment block
    comment_text_size = strlen(comment_text);
    if (fwrite(comment_text, sizeof(char), comment_text_size, file) 
            != comment_text_size * sizeof(char)) 
    {
        fprintf(stderr, "%s: could not write to open file %s\n", "mkcmain", filepath);
        free(filepath);
        die(EXIT_FAILURE, NULL);
    }

    // try to write C main file text to file
    cmain_text_size = strlen(cmain_text);
    if (fwrite(cmain_text, sizeof(char), cmain_text_size, file) 
            != cmain_text_size * sizeof(char)) 
    {
        fprintf(stderr, "%s: could not write to open file %s\n", "mkcmain", filepath);
        free(filepath);
        die(EXIT_FAILURE, NULL);
    }

    // try to close the file
    if (fclose(file)) {
        perror("fclose()");
        free(filepath);
        die(EXIT_FAILURE, "encountered error during attempt to close file");
    }

    fprintf(stdout, "%s: successfully created %s\n", "mkcmain", filepath);
    free(filepath);

    exit(EXIT_SUCCESS);
}
