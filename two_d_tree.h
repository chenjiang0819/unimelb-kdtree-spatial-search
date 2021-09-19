#include <stdio.h>

#ifndef TWO_D_TREE_H
#define TWO_D_TREE_H

/****************************************************************/

typedef struct node node_t;

struct node{
	//As the requirements of the assignment1, for saving the memory space, we try to store the data as int and float  
	int census_year;
	int block_id;
	int property_id;
	int base_property_id;
    char* clue_small_area;
    char* trading_name;
    int industry_code;
    char* industry_description;
    double x_coordinate;
	double y_coordinate;
	//for accuracy, we store the location as string instead of float or double.
	char* location;
	//the specific x,y coordinates for 2 d tree.
	double x_for_tree;
	double y_for_tree;
	//use the depth to determine whether the node is splited by x or y 
	int depth;
    node_t* left;
    node_t* right;
    node_t* next;
    node_t* list_foot;
};

typedef struct{
   node_t* root;
} tree_t;

/****************************************************************/

//function for making a new empty 2d tree
tree_t *make_empty_tree();

//function for inserting the new nodes into 2d tree. 
tree_t* insert_node(tree_t* tree, node_t* new_node);

//use the recursive function to insert the nodes
static node_t* recursive_insert(node_t* root, node_t* new_node);

//free all nodes in the 2d tree
static void free_nodes(node_t* root);

//free all memory space associated with the tree structure
void free_tree(tree_t* tree);

//compare the distance between current node and the root
double compare_distance(void* node_x, void* node_y, void* root_x, void* root_y);

//use the recursive and print the result
void recursive_print(FILE* output_file, node_t* root, char* one_key, int* times);

//we create a pointer to search the coordinates in the 2d tree 
void search_tree(tree_t* tree, char* one_key, FILE* output_file);

//use the recursive to search the nerest node in 2d tree
void recursive_search_nearest( node_t* root, node_t** search_node, double* distance, double x_for_serch, double y_for_serch, int* times, int switchs);

//use the recursive to search the nodes exsit in a specific threshold in 2d tree
void recursive_search_in_radius(FILE* output_file, node_t* root, double x_for_serch, double y_for_serch, 
	double threshold, int* times, char* one_key, int* exist, int switchs);

//read the key file and launch the searching  
void output_result(tree_t* tree, FILE* output_file);

//use the fprint to print the information in the output file.
void output_tree(node_t* curr, FILE* output_file, char* keep_one_key);

#endif 

