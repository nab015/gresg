#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

Options Options_new() {
  Options opts;
  opts.help_flag = 0;
  opts.version_flag = 0;
  opts.num_files = 0;
  opts.output_filename = NULL;
  opts.files = malloc(sizeof(opts.files));
  opts.files[0] = NULL;
  return opts;
}

void Options_add_output_filename(Options *opts, char *filename) {
  opts->output_filename = malloc(sizeof(char) * (strlen(filename) + 1));
  strcpy(opts->output_filename, filename);
}

int Options_has_output_filename(Options *opts) {
  int rc;
  if (opts->output_filename != NULL)
    rc = 1;
  else
    rc = 0;
  return rc;
}

int Options_get_output_filename(Options *opts, char **buffer) {
  int rc;
  if (Options_has_output_filename(opts)) {
    rc = 0;
    if (*buffer != NULL)
      free(*buffer);

    *buffer = malloc(sizeof(char) * (strlen(opts->output_filename) + 1));
    strcpy(*buffer, opts->output_filename);
  }

  else {
    rc = 1;
  }

  return rc;
}

int Options_append_filename(Options *opts, char *filename) {
  char **tmp;
  int rc;

  tmp = realloc(opts->files, sizeof(opts->files) * (opts->num_files + 2));
  if (tmp == NULL) {
    fprintf(stderr,
            "gresg: Error processing files: Couldn't allocate memory\n");
    rc = 0;
  }

  else {
    opts->files = tmp;
    opts->files[opts->num_files] =
        malloc(sizeof(char) * (strlen(filename) + 1));
    strcpy(opts->files[opts->num_files], filename);
    opts->files[++opts->num_files] = NULL;
    rc = 1;
    return rc;
  }
}

void Options_cleanup_memory(Options *opts) {
  for (int a = 0; a < opts->num_files; a++)
    free(opts->files[a]);

  free(opts->files);
}

int Options_has_help_flag(Options *opts) { return opts->help_flag; }

int Options_has_version_flag(Options *opts) { return opts->version_flag; }

Options parse_cmdline(int argc, char **argv) {
  Options opts;
  opts = Options_new();
  bool skip_next = false;
  for (int a = 1; a < argc; a++) {
    if (skip_next) {
      skip_next = false;
      continue;
    }

    if (argv[a][0] == '-') {
      if (argv[a][1] == '-') {
        char *longopt = argv[a];
        if (!strcmp(longopt, "--help")) {
          opts.help_flag = true;
          continue;
        }

        else if (!strcmp(longopt, "--version")) {
          opts.version_flag = true;
          continue;
        }

        else if (!strcmp(longopt, "--output")) {
          if (argv[a + 1] != NULL) {
            Options_add_output_filename(&opts, argv[a + 1]);
            skip_next = true;
          }

          else {
            fprintf(stderr, "Option --output requires an argument\n");
            exit(1);
          }
        }

        else if (!strcmp(longopt, "--")) {
          break;
        }

        else {
          fprintf(stderr, "Unknown option '%s'\n", longopt);
          fprintf(stderr, "Use --help for help\n");
          exit(1);
        }
      }

      else {
        int optlen = strlen(argv[a]);
        for (int b = 1; b < optlen; b++) {
          char shortopt = argv[a][b];
          if (shortopt == 'h') {
            opts.help_flag = true;
            continue;
          }

          else if (shortopt == 'V') {
            opts.version_flag = true;
            continue;
          }

          else if (shortopt == 'o') {
            if (argv[a + 1] != NULL) {
              Options_add_output_filename(&opts, argv[a + 1]);
              skip_next = true;
            }

            else {
              fprintf(stderr, "Option -o required an argument\n");
              exit(1);
            }
          }

          else {
            fprintf(stderr, "Unknown option '-%c'\n", shortopt);
            fprintf(stderr, "Use --help for help\n");
            exit(1);
          }
        }
      }
    }

    else {
      Options_append_filename(&opts, argv[a]);
      continue;
    }
  }

  if (opts.num_files == 0 && !(opts.help_flag || opts.version_flag)) {
    fprintf(stderr, "You must specify some files\n");
    exit(1);
  }

  return opts;
}

char **Options_get_files(Options *opts) { return opts->files; }
