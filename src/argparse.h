#ifndef HAVE_ARGPARSE_H
#define HAVE_ARGPARSE_H

#include <stdbool.h>

typedef struct {
  bool help_flag;
  bool version_flag;
  char **files;
  int num_files;
  char *output_filename;
} Options;

/* Free memory used by an Options object */
void Options_cleanup_memory(Options *opts);

/* Check if the help flag is set, if it is, this function will return true,
 * otherwise false */
int Options_has_help_flag(Options *opts);

/* Check if the version flag is set, if it is, this function will return true,
 * otherwise false */
int Options_has_version_flag(Options *opts);

/* Retrieve the output filename placing it in buffer */
int Options_get_output_filename(Options *opts, char **buffer);

/* Check if the Options struct has an output filename associated with it */
int Options_has_output_filename(Options *opts);

/* Retrieve a pointer to the array of filenames */
char **Options_get_files(Options *opts);

/* Parse the command line returning an Options object */
Options parse_cmdline(int argc, char **argv);

#endif /* HAVE_ARGPARSE_H */
