#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "argparse.h"
#include "writer.h"

void print_help()
{
    printf("Usage: gresg [OPTIONS] [FILES]\n\n");
    printf("Options:\n\n");
    printf("-h, --help           Display this help message\n");
    printf("-V, --version        Display version info\n");
}

void print_version()
{
    printf("gresg 2.0\n");
}

int main(int argc, char **argv)
{
    Options opts;
    opts = parse_cmdline(argc, argv);

    if(Options_has_help_flag(&opts))
    {
        print_help();
        return 0;
    }

    if(Options_has_version_flag(&opts))
    {
        print_version();
        return 0;
    }

    int status = write_xml_resources("gresources.xml", opts.files, opts.num_files);

    if(status == 0)
    {
        printf("Generating done, make sure to check and modify file if neccesary\n");
    }

    return status;
}
