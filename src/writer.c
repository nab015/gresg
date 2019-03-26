#include <stdio.h>
#include <stdlib.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlstring.h>

#include "writer.h"

int write_xml_resources(char *filename, char **files, int num_files)
{
    xmlTextWriter *writer;
    int rc;

    writer = xmlNewTextWriterFilename(filename, 0);
    if (writer == NULL)
    {
        fprintf(stderr, "Error: Could not create document writer\n");
        return -1;
    }

    rc = xmlTextWriterSetIndent(writer, 1);
    if (rc == -1)
    {
        fprintf(stderr, "Could not set indent level\n");
    }

    rc = xmlTextWriterStartDocument(writer, NULL, NULL, NULL);
    if (rc == -1)
    {
        fprintf(stderr, "Error creating document\n");
        goto failed;
    }

    rc = xmlTextWriterStartElement(writer, BAD_CAST "gresources");
    if (rc == -1)
    {
        fprintf(stderr, "Error creating toplevel gresources element\n");
        goto failed;
    }

    rc = xmlTextWriterStartElement(writer, BAD_CAST "gresource");
    if (rc == -1)
    {
        fprintf(stderr, "Error creating gresource bundle\n");
        goto failed;
    }

    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "prefix", BAD_CAST "/org/gtk/Example");
    if (rc == -1)
    {
        fprintf(stderr, "Could not set properties of gresource bundle\n");
        goto failed;
    }

   for(int a = 0; a < num_files; a++)
   {
       rc = xmlTextWriterWriteElement(writer, BAD_CAST "file", BAD_CAST files[a]);
       if (rc == -1)
       {
           fprintf(stderr, "Could not write element for file '%s'\n", files[a]);
           goto failed;
       }
   } 

   /* rc = xmlTextWriterEndElement(writer); // gresource
   if (rc == -1)
   {
       fprintf(stderr, "Could not close gresource bundle\n");
       goto failed;
   }

   rc = xmlTextWriterEndElement(writer); // gresources
   if (rc == -1)
   {
       fprintf(stderr, "Could not close gresources element\n");
       goto failed;
   } */

   rc = xmlTextWriterEndDocument(writer);
   if (rc == -1)
   {
       fprintf(stderr, "Error closing document\n");
       goto failed;
   }

   xmlFreeTextWriter(writer);
   xmlCleanupParser();
   return 0;

failed:
    xmlFreeTextWriter(writer);
    return -1;
}

/* Used for testing */
/* int main()
{
    char *files[] = { "hello.txt", "goodbye.txt" };
    int num_files = 2;
    int rc = write_xml_resources("output.xml", files, num_files);
    if (rc == -1)
    {
        fprintf(stderr, "Something went wrong\n");
        return 1;
    }

    return 0;
} */
