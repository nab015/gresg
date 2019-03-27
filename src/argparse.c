#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

int Options_get_file_index = 0;

Options Options_new()
{
    Options opts;
    opts.help_flag = false;
    opts.version_flag = false;
    opts.num_files = 0;
    opts.output_filename = NULL;
    opts.files = malloc(sizeof(opts.files));
    opts.files[0] = NULL;
    return opts;
}

void Options_add_output_filename(Options *opts, char *filename)
{
    opts->output_filename = malloc(sizeof(char) * (strlen(filename) + 1));
    strcpy(opts->output_filename, filename);
}

int Options_get_output_filename(Options *opts, char **buffer)
{
    if(opts->output_filename == NULL)
        return -1;

    if(*buffer != NULL)
        free(*buffer);

    *buffer = malloc(sizeof(char) * (strlen(opts->output_filename) + 1));
    strcpy(*buffer, opts->output_filename);
    return 0;
}

bool Options_append_filename(Options *opts, char *filename)
{
    char **tmp;

    tmp = realloc(opts->files, sizeof(opts->files) * (opts->num_files + 2));
    if(tmp == NULL)
    {
        fprintf(stderr, "gresg: Error: Couldn't allocate memory\n");
        return false;
    }

    opts->files = tmp;
    opts->files[opts->num_files] = malloc(sizeof(char) * (strlen(filename) + 1));
    strcpy(opts->files[opts->num_files], filename);
    opts->files[++opts->num_files] = NULL;
    return true;
}

void Options_cleanup_memory(Options *opts)
{
    for(int a = 0; a < opts->num_files; a++)
    {
        free(opts->files[a]);
    }

    free(opts->files);
}

bool Options_has_help_flag(Options *opts)
{
    return opts->help_flag;
}

bool Options_has_version_flag(Options *opts)
{
    return opts->version_flag;
}

char *Options_get_filename(Options *opts, bool *has_value)
{
    if(opts->files[Options_get_file_index] == NULL)
        *has_value = false;
    else
        *has_value = true;
    return opts->files[Options_get_file_index];
}

Options parse_cmdline(int argc, char **argv)
{
    Options opts;
    opts = Options_new();
    bool skip_next = false;
    for(int a = 1; a < argc; a++)
    {
        if(skip_next)
        {
            skip_next = false;
            continue;
        }

        if(argv[a][0] == '-')
        {
            if(argv[a][1] == '-')
            {
                char *longopt = argv[a];
                if(!strcmp(longopt, "--help"))
                {
                    opts.help_flag = true;
                    continue;
                }
                
                else if(!strcmp(longopt, "--version"))
                {
                    opts.version_flag = true;
                    continue;
                }

                else if(!strcmp(longopt, "--output"))
                {
                    if(argv[a+1] != NULL)
                    {
                        Options_add_output_filename(&opts, argv[a+1]);
                        skip_next = true;
                    }

                    else
                    {
                        fprintf(stderr, "Option --output requires an argument\n");
                        exit(1);
                    }
                }
                
                else if(!strcmp(longopt, "--"))
                {
                    break;
                }

                else
                {
                    fprintf(stderr, "Unknown option '%s'\n", longopt);
                    fprintf(stderr, "Use --help for help\n");
                    exit(1);
                }
            }

            else
            {
                int optlen = strlen(argv[a]);
                for(int b = 1; b < optlen; b++)
                {
                    char shortopt = argv[a][b];
                    if(shortopt == 'h')
                    {
                        opts.help_flag = true;
                        continue;
                    }

                    else if(shortopt == 'V')
                    {
                        opts.version_flag = true;
                        continue;
                    }

                    else if(shortopt == 'o')
                    {
                        if(argv[a+1] != NULL)
                        {
                            Options_add_output_filename(&opts, argv[a+1]);
                            skip_next = true;
                        }

                        else
                        {
                            fprintf(stderr, "Option -o required an argument\n");
                            exit(1);
                        }
                    }

                    else
                    {
                        fprintf(stderr, "Unknown option '-%c'\n", shortopt);
                        fprintf(stderr, "Use --help for help\n");
                        exit(1);
                    }
                }
            }
        }

        else
        {
            Options_append_filename(&opts, argv[a]);
            continue;
        }
    }

    if(opts.num_files == 0 && !(opts.help_flag || opts.version_flag))
    {
        fprintf(stderr, "You must specify some files\n");
        exit(1);
    }

    return opts;
}
