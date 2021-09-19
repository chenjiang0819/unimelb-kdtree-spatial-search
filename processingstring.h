#include <stdio.h>
#include "two_d_tree.h"

#ifndef PROCESSINGSTRING_H
#define PROCESSINGSTRING_H

/****************************************************************/

//processing the trading name to prevent if there is special case.
void processing_trading_name(char* buffer, int* count, node_t* new_node);

//processing the ndustry description to prevent if there is special case.
void processing_industry_description(char* buffer, int* count, node_t* new_node);

//processing the clue small area to prevent if there is special case.
void processing_clue_small_area(char* buffer, int* count, node_t* new_node);

#endif 