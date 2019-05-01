#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"
#include "writer.h"

void print_help() {
  printf("Usage: gresg [OPTIONS] [FILES]\n\n");
  printf("Options:\n\n");
  printf("-o, --output         The name of the file to output to\n");
  printf("-h, --help           Display this help message\n");
  printf("-V, --version        Display version info\n");
}

void print_version() { printf("gresg 2.0\n"); }

int main(int argc, char **argv) {
  Options opts;
  opts = parse_cmdline(argc, argv);

  if (Options_has_help_flag(&opts)) {
    print_help();
    return 0;
  }

  if (Options_has_version_flag(&opts)) {
    print_version();
    return 0;
  }

  char *outfile = NULL;
  int rc = Options_get_output_filename(&opts, &outfile);
  if (rc == -1) {
    outfile = malloc(sizeof(char) * 15);
    strcpy(outfile, "gresources.xml");
  }

  int status = write_xml_resources(outfile, Options_get_files(&opts));

  if (status == 0) {
    printf(
        "Generating done, make sure to check and modify file if neccesary\n");
  }

  return status;
}
