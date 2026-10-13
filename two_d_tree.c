#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include "two_d_tree.h"

#define TRUE 1
#define NULLBIT 1
#define RETURN 1
#define SPACE 1
#define BEGINING 0
#define IMPOSSIBLE -1
#define TURN_ON -1

static node_t *recursive_insert(node_t *root,node_t *new_node);
static void free_nodes(node_t *root);

/****************************************************************/

//function for making a new empty 2d tree
tree_t *make_empty_tree(void){
    tree_t *tree;
	tree = (tree_t*)malloc(sizeof(*tree));
	if (!tree) return NULL;
	tree -> root = NULL;
	return tree;
}

//function for inserting the new nodes into 2d tree. 
tree_t* insert_node(tree_t* tree, node_t* new_node){
	tree -> root = recursive_insert(tree -> root, new_node);
    return tree;
}
//use the recursive function to insert the nodes
static node_t* recursive_insert(node_t* root, node_t* new_node){
	if(root == NULL){
        return new_node;
    }else if((new_node -> depth) % 2 == BEGINING){
    	(new_node -> depth) ++;
    	if((new_node -> x_for_tree) < (root -> x_for_tree)){
    		root -> left = recursive_insert(root -> left, new_node);
    	}else{
    		//put the nodes that have same location in a linked list
    		if(((new_node -> x_for_tree) == (root -> x_for_tree)) && ((new_node -> y_for_tree) == (root -> y_for_tree))){
    			if(root -> list_foot == NULL){
    				root -> next = new_node;
    				root -> list_foot = new_node;
    			}else{
    				root -> list_foot -> next = new_node;
    				root -> list_foot = new_node;
    			}
    			return root;
    		}
    		root -> right = recursive_insert(root -> right, new_node);
    	}
    }else if ((new_node -> depth) % 2 == 1){
    	(new_node -> depth) ++;
       	if((new_node -> y_for_tree) < (root -> y_for_tree)){
    		root -> left = recursive_insert(root -> left, new_node);
    	}else{
    		//put the nodes that have same location in a linked list
    		if(((new_node -> x_for_tree) == (root -> x_for_tree))&& ((new_node -> y_for_tree) == (root -> y_for_tree))){
    			if(root -> list_foot == NULL){
    				root -> next = new_node;
    				root -> list_foot = new_node;
    			}else{
    				root -> list_foot -> next = new_node;
    				root -> list_foot = new_node;
    			}
    			return root;
    		}
    		root -> right = recursive_insert(root -> right, new_node);
    	}
    }
    return root;
}

static int query_values(const char *key,double values[3],int *count) {
    const char *cursor=key;*count=0;
    while (*cursor) {
        while (isspace((unsigned char)*cursor)) cursor++;
        if (!*cursor) break;
        if (*count==3) return 0;
        char *end;errno=0;double value=strtod(cursor,&end);
        if (end==cursor || errno || !isfinite(value) || (*end && !isspace((unsigned char)*end))) return 0;
        values[(*count)++]=value;cursor=end;
    }
    return (*count==2 || *count==3) && (*count!=3 || values[2]>=0);
}

//read the key file and launch the searching  
int output_result(tree_t *tree, FILE *output_file){
    char *line=NULL;size_t capacity=0;int ok=1;
    while (getline(&line,&capacity,stdin)>=0) {
        size_t n=strlen(line);
        while (n && (line[n-1]=='\n' || line[n-1]=='\r')) line[--n]='\0';
        if (!n) continue;
        double values[3];int count;
        if (n>4096 || !query_values(line,values,&count)) { ok=0;break; }
        search_tree(tree,line,output_file);
        if (ferror(output_file)) {ok=0;break;}
    }
    if (ferror(stdin)) ok=0;
    free(line);return ok;
}

// we search the coordinates in the 2d tree 
void search_tree(tree_t* tree, char* one_key, FILE* output_file){
	int times = BEGINING;
	printf("%s", one_key);
	//use the recursive to traverse the 2d tree
	recursive_print(output_file, tree -> root, one_key, &times);
    // printf search times 
    fprintf(output_file,"\n");
    printf( " --> ");
    printf("%d\n", times);
}

//use the recursive and print the result
void recursive_print(FILE *output_file,node_t *root,char *one_key,int *times){
    double values[3];int count;
    if (!query_values(one_key,values,&count)) return;
    if (!root) {fprintf(output_file,"%s --> NOTFOUND",one_key);return;}
    if (count==2) {
        node_t *nearest=root;
        double distance=compare_distance(&root->x_for_tree,&root->y_for_tree,&values[0],&values[1]);
        recursive_search_nearest(root,&nearest,&distance,values[0],values[1],times,TRUE);
        output_tree(nearest,output_file,one_key);
    } else {
        int exist=0;
        recursive_search_in_radius(output_file,root,values[0],values[1],values[2],times,one_key,&exist,TRUE);
        if (!exist) fprintf(output_file,"%s --> NOTFOUND",one_key);
    }
}


//this algorithm was created by mayself which will increase some complexity of other coordinates to reduce the time complexity of 0,0. :)
//use the recursive to search the nerest node in 2d tree
void recursive_search_nearest(node_t *root,node_t **search_node,double *distance,
    double x_for_serch,double y_for_serch,int *times,int switchs){
    (void)switchs;if (!root) return;
    (*times)++;
    double candidate=compare_distance(&root->x_for_tree,&root->y_for_tree,&x_for_serch,&y_for_serch);
    if (candidate<*distance) {*distance=candidate;*search_node=root;}
    double delta=root->depth%2 ? y_for_serch-root->y_for_tree : x_for_serch-root->x_for_tree;
    node_t *near=delta<0 ? root->left : root->right;
    node_t *far=delta<0 ? root->right : root->left;
    recursive_search_nearest(near,search_node,distance,x_for_serch,y_for_serch,times,switchs);
    if (fabs(delta)<=*distance)
        recursive_search_nearest(far,search_node,distance,x_for_serch,y_for_serch,times,switchs);
}

//use the recursive to search the nodes exsit in a specific threshold in 2d tree
void recursive_search_in_radius(FILE *output_file,node_t *root,double x_for_serch,double y_for_serch,
    double threshold,int *times,char *one_key,int *exist,int switchs){
    (void)switchs;if (!root) return;
    (*times)++;
    double delta=root->depth%2 ? y_for_serch-root->y_for_tree : x_for_serch-root->x_for_tree;
    if (delta<=threshold) recursive_search_in_radius(output_file,root->left,x_for_serch,y_for_serch,threshold,times,one_key,exist,switchs);
    if (delta>=-threshold) recursive_search_in_radius(output_file,root->right,x_for_serch,y_for_serch,threshold,times,one_key,exist,switchs);
    if (compare_distance(&root->x_for_tree,&root->y_for_tree,&x_for_serch,&y_for_serch)<=threshold) {
        (*exist)++;output_tree(root,output_file,one_key);
    }
}

//use the fprint to print the information in the output file.
void output_tree(node_t* curr, FILE* output_file, char* keep_one_key){
	// Traverse the duplicate-coordinate bucket without mutating its insertion tail.
	node_t *item=curr;
	while(item != NULL){
		fprintf(output_file,"%s", keep_one_key);
		fprintf(output_file," --> ");
	    fprintf(output_file,"Census year: %d || ", item ->census_year);
		fprintf(output_file,"Block ID: %d || ", item ->block_id);
		fprintf(output_file,"Property ID: %d || ", item ->property_id);
		fprintf(output_file,"Base property ID: %d || ", item ->base_property_id);
		fprintf(output_file,"CLUE small area: %s || ", item ->clue_small_area);
		fprintf(output_file,"Trading Name: %s || " , item ->trading_name);
		fprintf(output_file,"Industry (ANZSIC4) code: %d || ", item ->industry_code);
		fprintf(output_file,"Industry (ANZSIC4) description: %s || ", item ->industry_description);
		fprintf(output_file,"x coordinate: %0.5lf || ", item ->x_coordinate);
		fprintf(output_file,"y coordinate: %0.5lf || ", item ->y_coordinate);
		fprintf(output_file,"Location: (%s || \n", item ->location);
		item = item -> next;
	}
}

//compare the distance between current node and the root
double compare_distance(void* node_x, void* node_y, void* root_x, void* root_y){
	double result;
	//comparing
	result = hypot(*(double*)node_x-*(double*)root_x,*(double*)node_y-*(double*)root_y);
	return result;
}

//free all memory space associated with the tree structure
void free_tree(tree_t* tree){
	assert(tree != NULL);
	free_nodes(tree -> root);
	free(tree);
	tree = NULL;
}

//free all nodes in the 2d tree
static void free_nodes(node_t* root){
	node_t* pre;
	if(root != NULL){
		free_nodes(root -> left);
		free_nodes(root -> right);
		//free the momeries that malloced for the properties of the nodes and point it to NULL to prevent the wild pointers
		while(root != NULL){
			free(root -> trading_name);
	       	root -> trading_name = NULL;
	        free(root -> clue_small_area);
	        root -> clue_small_area = NULL;
	        free(root -> industry_description);
	        root -> industry_description = NULL;
	        free(root -> location);
	        root -> location = NULL;
	        pre = root;
	        root = root -> next;
	        free(pre);
			pre = NULL;
		}
	}
}
