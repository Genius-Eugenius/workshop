/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Test Environment: Get capture files path.
 *
 * Parse Logger configuration file to get path to capture logs. Print the
 * directory path to stdout.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <yaml.h>

#include "te_defs.h"
#include "te_yaml.h"
#include "logger_cnf.h"

/* Path to capture files */
char *path = NULL;

/**
 * Extract "path" value from a config section.
 *
 * @param d         YAML document
 * @param node      YAML node for the section
 */
static void
te_log_extract_section(yaml_document_t *d, yaml_node_t *node)
{
    yaml_node_pair_t *pair;

    for (pair = node->data.mapping.pairs.start;
         pair < node->data.mapping.pairs.top; pair++)
    {
        yaml_node_t *k = yaml_document_get_node(d, pair->key);
        yaml_node_t *v = yaml_document_get_node(d, pair->value);
        const char  *key = te_yaml_scalar_value(k);
        const char  *val = te_yaml_scalar_value(v);

        if (key != NULL && val != NULL && strcmp(key, "path") == 0)
        {
            free(path);
            path = strdup(val);
        }
    }
}

/**
 * Extract "path" value from a YAML config file.
 *
 * @param filename     file name
 */
static void
te_log_extract_yaml(const char *filename)
{
    FILE             *input;
    yaml_parser_t     parser;
    yaml_document_t   document;
    yaml_node_t      *root             = NULL;
    yaml_node_t      *sniffers_default = NULL;
    yaml_node_t      *sniffers         = NULL;
    yaml_node_pair_t *pair;

    input = fopen(filename, "r");
    if (input == NULL)
    {
        perror("Failed to open YAML config file");
        return;
    }

    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, input);
    if (yaml_parser_load(&parser, &document) == 0)
    {
        fputs("Failed to load YAML document\n", stderr);
        yaml_parser_delete(&parser);
        fclose(input);
        return;
    }
    yaml_parser_delete(&parser);
    fclose(input);

    root = yaml_document_get_root_node(&document);
    if (root == NULL)
    {
        fputs("Failed to get root YAML node, assuming empty config\n", stderr);
        yaml_document_delete(&document);
        return;
    }

    /* Do nothing if config is empty */
    if (root->type == YAML_SCALAR_NODE && root->data.scalar.length == 0)
        return;

    if (root->type != YAML_MAPPING_NODE)
    {
        fputs("Root YAML node is not a mapping\n", stderr);
        return;
    }

    for (pair = root->data.mapping.pairs.start;
         pair < root->data.mapping.pairs.top; pair++)
    {
        yaml_node_t *k = yaml_document_get_node(&document, pair->key);
        yaml_node_t *v = yaml_document_get_node(&document, pair->value);
        const char  *key = te_yaml_scalar_value(k);

        if (key == NULL)
            continue;

        if (strcmp(key, "sniffers_default") == 0)
            sniffers_default = v;
        else if (strcmp(key, "sniffers") == 0)
            sniffers = v;
    }

    if (sniffers_default != NULL)
        te_log_extract_section(&document, sniffers_default);

    if (sniffers != NULL)
        te_log_extract_section(&document, sniffers);

    yaml_document_delete(&document);
}

int
main(int argc, char **argv)
{
    const char *filename;

    if (argc != 2)
    {
        fputs("Usage: te_log_get_path logger.conf\n", stderr);
        return EXIT_FAILURE;
    }
    filename = argv[1];

    switch(get_cfg_file_type(filename))
    {
        case CFG_TYPE_ERROR:
            fprintf(stderr,
                    "Failed to determine config file format: %s\n", strerror(errno));
            return EXIT_FAILURE;
        case CFG_TYPE_EMPTY:
            return EXIT_SUCCESS;
        case CFG_TYPE_YAML:
            te_log_extract_yaml(filename);
            break;
        case CFG_TYPE_OTHER:
            fputs("Unknown config file format\n", stderr);
            return EXIT_FAILURE;
     }

    if (path != NULL)
    {
        puts(path);
        free(path);
    }
    return EXIT_SUCCESS;
}
