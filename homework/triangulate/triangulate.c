/**
 * triangulate
 * -----
 * Write a program to find the lowest-cost triangulation of a convex polygon.
 * -----
 * Princeton Ferro
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define distance(v,i,j) (j-i>=2 ? hypotf(v[i][0]-v[j][0],v[i][1]-v[j][1]) : 0)

struct wt {
	float w;
	int k;
};

float minw(struct wt **weight, float (*v)[2], int n);

void print_tris(struct wt **weight, int i, int j);

int main(int argc, char *argv[]) {
	int n;
	float (*points)[2];
	struct wt **weight;

	scanf("%d", &n);
	weight = malloc(n * sizeof(*weight));	
	points = calloc(n, sizeof(*points));
	for (int i=0; i<n; ++i) {
		scanf("%f %f", &points[i][0], &points[i][1]);
		weight[i] = malloc(n * sizeof(**weight));
	}
	
	minw(weight, points, n);

	printf("%f\n", weight[0][n-1].w);
	print_tris(weight, 0, n-1);

	for (int i=0; i<n; ++i)
		free(weight[i]);
	free(weight);
	free(points);
	return 0;
}

/**
 * approach:
 * 	W(i,j) = distance(i,j) + min(i < k < j){ W(i,k) + W(k,j) }
 * 	where W = weight
 * 	
 * 	1. start from W(n-2,n-1), where i=n-2 and j=n-1
 * 	2. given our edge, i->j, loop k from i+1 to j-1
 * 		2a. for each triangle ikj, W(i,k) and W(k,j) are already set
 * 		to the min triangulation for the points from i->k and k->j
 * 		2b. therefore, W(i,j) is the sum of the minimum of those, plus
 * 			3a. distance(i,j) if ij is an interior edge, or i and j
 * 			are not first and last points
 * 			3b. 0 otherwise
 * 	3. repeat 2 for all j (i < j < n)
 * 	4. repeat 3 for all i (0 <= i < n)
 *
 */
float minw(struct wt **weight, float (*v)[2], int n)
{
	for (int i=n-1; i>=0; --i)
		for (int j=i+1; j<n; ++j) {
			float min = 0, val;
			int mink = -1;

			for (int k=i+1; k<j; ++k) {
				val = weight[i][k].w + weight[k][j].w;
				if (min == 0 || val < min) {
					min = val;
					mink = k;
				}
			}

			if (i == 0 && j == n-1)
				weight[i][j].w = min;
			else 
				weight[i][j].w = min + distance(v,i,j);
			weight[i][j].k = mink;
		}
	return weight[0][n-1].w;
}

void print_tris(struct wt **weight, int i, int j)
{
	if (weight[i][j].k != -1) {
		int k = weight[i][j].k;
		if (weight[i][k].w != 0)
			printf("%d %d\n", i, k);
		if (weight[k][j].w != 0)
			printf("%d %d\n", k, j);
		print_tris(weight, i, k);
		print_tris(weight, k, j);
	}
}
