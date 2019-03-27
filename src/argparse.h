#ifndef HAVE_ARGPARSE_H
#define HAVE_ARGPARSE_H

#include <stdbool.h>

typedef struct
{
    bool help_flag;
    bool version_flag;
    char **files;
    int num_files;
    char *output_filename;
} Options;

/* Create a new instance of the Options object */
Options Options_new();

/* Append a filename to the Options type referenced by *opts */
bool Options_append_filename(Options *opts, char *filename);

/* Free memory used by an Options object */
void Options_cleanup_memory(Options *opts);

/* Check if the help flag is set, if it is, this function will return true, otherwise false */
bool Options_has_help_flag(Options *opts);

/* Check if the version flag is set, if it is, this function will return true, otherwise false */
bool Options_has_version_flag(Options *opts);

/* Get an item from the files array */
char *Options_get_filename(Options *opts, bool *has_value);

Options parse_cmdline(int argc, char **argv);

void Options_add_output_filename(Options *opts, char *filename);

int Options_get_output_filename(Options *opts, char **buffer);

#endif /* HAVE_ARGPARSE_H */
