/* Original KD-tree coursework implementation by Chen Jiang.
 * 2026 maintenance bounds CSV/input parsing while retaining the node/tree model. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>
#include "two_d_tree.h"
#include "processingstring.h"

static int integer(const char *text, int *value) {
    if (!*text) { *value=0;return 1; }
    char *end;errno=0;long n=strtol(text,&end,10);
    if (errno || *end || n<0 || n>INT_MAX) return 0;
    *value=(int)n;return 1;
}
static int number(const char *text, double *value) {
    if (!*text) { *value=0;return 1; }
    char *end;errno=0;double n=strtod(text,&end);
    if (errno || end==text || *end || !isfinite(n)) return 0;
    *value=n;return 1;
}
static int location(const char *text, double *x, double *y) {
    char *end;errno=0;
    if (*text++!='(') return 0;
    *y=strtod(text,&end);if (end==text || errno || !isfinite(*y)) return 0;
    while (isspace((unsigned char)*end)) end++;
    if (*end++!=',') return 0;
    text=end;errno=0;*x=strtod(text,&end);
    if (end==text || errno || !isfinite(*x)) return 0;
    while (isspace((unsigned char)*end)) end++;
    return *end==')' && end[1]=='\0';
}
static void free_record(node_t *node) {
    if (!node) return;
    free(node->clue_small_area);free(node->trading_name);
    free(node->industry_description);free(node->location);free(node);
}

int main(int argc, char **argv) {
    if (argc!=3) { fprintf(stderr,"Usage: map2 DATA.csv OUTPUT.txt < QUERIES.txt\n");return EXIT_FAILURE; }
    struct stat input_stat,output_stat;
    if (!stat(argv[1],&input_stat) && !stat(argv[2],&output_stat) &&
        input_stat.st_dev==output_stat.st_dev && input_stat.st_ino==output_stat.st_ino) {
        fprintf(stderr,"Input and output must be different files\n");return EXIT_FAILURE;
    }
    FILE *input=fopen(argv[1],"r"), *output=NULL;
    tree_t *tree=make_empty_tree();
    char *line=NULL,*fields[11];size_t capacity=0;node_t *node=NULL;
    int success=0;
    if (!input || !tree) goto done;
    if (getline(&line,&capacity,input)<0 || !parse_csv_fields(line,fields,11) ||
        strcmp(fields[0],"Census year") || strcmp(fields[10],"Location")) goto done;
    while (getline(&line,&capacity,input)>=0) {
        if (strlen(line)>1024*1024 || !parse_csv_fields(line,fields,11)) goto done;
        node=calloc(1,sizeof(*node));if (!node) goto done;
        if (!integer(fields[0],&node->census_year) || !integer(fields[1],&node->block_id) ||
            !integer(fields[2],&node->property_id) || !integer(fields[3],&node->base_property_id) ||
            !integer(fields[6],&node->industry_code) || !number(fields[8],&node->x_coordinate) ||
            !number(fields[9],&node->y_coordinate) || !location(fields[10],&node->x_for_tree,&node->y_for_tree)) goto done;
        node->clue_small_area=strdup(fields[4]);node->trading_name=strdup(fields[5]);
        node->industry_description=strdup(fields[7]);node->location=strdup(fields[10]+1);
        if (!node->clue_small_area || !node->trading_name || !node->industry_description || !node->location) goto done;
        insert_node(tree,node);node=NULL;
    }
    if (ferror(input)) goto done;
    output=fopen(argv[2],"w");if (!output) goto done;
    success=output_result(tree,output);
done:
    if (!success) fprintf(stderr,"Invalid input or I/O failure: expected 11-field CSV and finite x y [radius] queries\n");
    free_record(node);free(line);
    if (input && fclose(input)) success=0;
    if (output && fclose(output)) success=0;
    if (tree) free_tree(tree);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
