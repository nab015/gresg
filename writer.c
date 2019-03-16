#include <stdio.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>

#define MY_ENCODING "UTF-8"

xmlChar* ConvertInput(const char *in, const char *encoding);

int writeXmlFile(const char* uri)
{
    int rc;
    xmlTextWriterPtr writer;
    xmlChar* text;

    writer = xmlNewTextWriterFilename(uri, 0);
    if(writer == NULL)
    {
        fprintf(stderr, "Error creating XML writer\n");
        return 1;
    }

    rc = xmlTextWriterSetIndent(writer, 1);
    if(rc < 0)
    {
        fprintf(stderr, "Error setting indentation\n");
        return 1;
    }

    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if(rc < 0)
    {
        fprintf(stderr, "Error creating the XML document\n");
        return 1;
    }

    /* Open main gresources element */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "gresources");
    if(rc < 0)
    {
        fprintf(stderr, "Error creating gresources element\n");
        return 1;
    }

    /* Open gresource element */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "gresource");
    if(rc < 0)
    {
        fprintf(stderr, "Error creating gresource element\n");
        return 1;
    }

    /* Create a file element with file name main.ui */
    text = ConvertInput("main.ui", MY_ENCODING);
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "file", "%s", text);
    if(rc < 0)
    {
        fprintf(stderr, "Error creating file element\n");
        return 1;
    }

    /* Close gresource element */
    rc = xmlTextWriterEndElement(writer);
    if(rc < 0)
    {
        fprintf(stderr, "Error closing elements\n");
        return 1;
    }

    /* Close gresources element */
    rc = xmlTextWriterEndElement(writer);
    if(rc < 0)
    {
        fprintf(stderr, "Error closing elements\n");
        return 1;
    }

    /* End document */
    rc = xmlTextWriterEndDocument(writer);
    if(rc < 0)
    {
        fprintf(stderr, "Error closing document\n");
        return 1;
    }

    xmlFreeTextWriter(writer);
    xmlCleanupParser();
    return 0;
}

xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}
