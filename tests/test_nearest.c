#include "../two_d_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
int main(void) {
    tree_t *tree=make_empty_tree(); double x[80],y[80];
    unsigned seed=2026;
    for(int i=0;i<80;i++) {
        seed=seed*1664525u+1013904223u;x[i]=(double)(seed%2001)/10-100;
        seed=seed*1664525u+1013904223u;y[i]=(double)(seed%2001)/10-100;
        node_t *n=calloc(1,sizeof(*n));if(!n)return 2;
        n->x_for_tree=x[i];n->y_for_tree=y[i];insert_node(tree,n);
    }
    int failures=0;
    for(int a=-90;a<=90;a+=7) for(int b=-90;b<=90;b+=11) {
        double expected=INFINITY;for(int i=0;i<80;i++) {double d=hypot(x[i]-a,y[i]-b);if(d<expected)expected=d;}
        node_t *nearest=tree->root;double distance=hypot(nearest->x_for_tree-a,nearest->y_for_tree-b);int count=0;
        recursive_search_nearest(tree->root,&nearest,&distance,a,b,&count,1);
        if(fabs(distance-expected)>1e-8) {if(!failures)fprintf(stderr,"query %d %d: nearest distance %.8f; oracle %.8f\n",a,b,distance,expected);failures++;}
    }
    free_tree(tree);if(failures)fprintf(stderr,"%d nearest-query mismatches\n",failures);
    else puts("442 exhaustive nearest-query checks passed");
    return failures?1:0;
}
