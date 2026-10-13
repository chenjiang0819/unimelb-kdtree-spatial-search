#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processingstring.h"

/* The original quoted-field handling is retained as a bounded character scan.
 * Decode into the same buffer; the read cursor never trails the write cursor. */
static char *read_field(char **cursor, int *separator) {
    char *start=*cursor, *read=start, *write=start;
    if (*read=='"') {
        int closed=0; read++;
        while (*read && *read!='\r' && *read!='\n') {
            if (*read=='"') {
                if (read[1]=='"') { *write++='"';read+=2; }
                else { read++;closed=1;break; }
            } else *write++=*read++;
        }
        if (!closed) return NULL;
        while (*read==' ' || *read=='\t') read++;
        if (*read && *read!=',' && *read!='\r' && *read!='\n') return NULL;
    } else {
        while (*read && *read!=',' && *read!='\r' && *read!='\n') {
            if (*read=='"') return NULL;
            *write++=*read++;
        }
    }
    *separator=*read==',';
    char *next=read;
    if (*separator) next++;
    else {
        while (*next=='\r' || *next=='\n') next++;
        if (*next) return NULL;
    }
    *write='\0'; *cursor=next; return start;
}

int parse_csv_fields(char *line, char **fields, size_t expected) {
    char *cursor=line;
    for (size_t i=0;i<expected;i++) {
        int separator;
        fields[i]=read_field(&cursor,&separator);
        if (!fields[i] || (i+1<expected && !separator) || (i+1==expected && separator)) return 0;
    }
    return 1;
}

static void assign_field(char *buffer, int *count, char **destination) {
    char *cursor=buffer+*count; int separator;
    char *field=read_field(&cursor,&separator);
    *destination=field ? strdup(field) : NULL;
    *count=(int)(cursor-buffer);
}

void processing_trading_name(char *buffer, int *count, node_t *node) {
    assign_field(buffer,count,&node->trading_name);
}
void processing_industry_description(char *buffer, int *count, node_t *node) {
    assign_field(buffer,count,&node->industry_description);
}
void processing_clue_small_area(char *buffer, int *count, node_t *node) {
    assign_field(buffer,count,&node->clue_small_area);
}
