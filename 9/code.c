#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CityLen 64

typedef struct _Distance {
	char	Start[CityLen];
	char	End[CityLen];
	int	Dist;
} Distance;

Distance *D=NULL;
size_t NumD=0, MaxD=0;

typedef struct _CityList {
	char		City[CityLen];
	struct _CityList	*Next;
} CityList;

CityList Cities={{0},NULL};

typedef struct _TreeNode {
	char			City[CityLen];
	int			StepDist;
	int			TotalDist;
	int			NumChildren;
	struct _TreeNode	*Children;
} TreeNode;

int GetDistance(const char *From, const char *To) {
	int i;

	for(i=0;i<NumD;i++) {
		if(	(!strcmp(From, D[i].Start) || !strcmp(From, D[i].End))
		   &&	(!strcmp(To, D[i].Start) || !strcmp(To, D[i].End))
		) {
			return D[i].Dist;
		}
	}
	
	printf("Couldn't get distance from '%s' to '%s'\n", From, To);
	exit(1);
	return -1;
}

CityList *InsertCityList(CityList *List, const char *City) {
	CityList *v, *e=NULL;

	for(v=List; v; v=v->Next) {
		if(!v->City[0]) {
			if(!e) {
				e=v;
			}
		} else if(!strcmp(City, v->City)) {
			return NULL;
		}
		if(!v->Next) {
			if(e) {
				v=e;
			} else {
				v->Next = (CityList*)malloc(sizeof(*v));
				v = v->Next;
			}
			memset(v, 0, sizeof(*v));
			strncpy(v->City, City, sizeof(v->City));
			return v;
		}
	}
	return NULL;
}

int CityInList(CityList *List, const char *City) {
	CityList *v;

	for(v=List; v; v=v->Next) {
		if(!strcmp(City, v->City)) {
			return 1;
		}
	}
	return 0;
}

void CreateTree(TreeNode *Parent, CityList *Visited) {
	TreeNode *Children=NULL;
	int NumChildren=0;
	CityList *C, *W;

	for(C = &Cities; C; C=C->Next) {
		if(!C->City[0] || CityInList(Visited, C->City)) {
			continue;
		}
		Children = (TreeNode*)realloc(Children, (NumChildren+1) * sizeof(*Children));
		strncpy(Children[NumChildren].City, C->City, sizeof(Children->City));
		if(Parent->City[0]) {
			Children[NumChildren].StepDist = GetDistance(Parent->City, Children[NumChildren].City);
		} else {
			Children[NumChildren].StepDist = 0;
		}
		Children[NumChildren].TotalDist = Parent->TotalDist + Children[NumChildren].StepDist;

		W=InsertCityList(Visited, C->City);
		CreateTree(&Children[NumChildren], Visited);
		W->City[0]=0;
		
		NumChildren++;
	}
	Parent->Children=Children;
	Parent->NumChildren=NumChildren;
}

TreeNode *DescendTree(TreeNode *node, int HighLow) {
	TreeNode *best=NULL, *working;
	int i;
	
	for(i=0;i<node->NumChildren;i++) {
		working = DescendTree(&node->Children[i], HighLow);
		if(!working) {
			continue;
		}
		if(!best || (HighLow && working->TotalDist > best->TotalDist) ||
			(!HighLow && working->TotalDist < best->TotalDist)
		) {
			best=working;
		}
	}
	if(!best) {
		return node;
	}
	return best;
}

int main(int argc, char *argv[]) {
	static char buffer[1024];
	TreeNode Root={{0}, 0, 0, 0, NULL}, *b;
	CityList v={{0},NULL};

	while(fgets(buffer, sizeof(buffer), stdin)) {
		char *from, *to, *diststr, *c;
		int Dist;

		from=buffer;
		c=strchr(buffer, ' ');
		*c=0;
		c=strchr(c+1, ' ');
		to=c+1;
		c=strchr(to, ' ');
		*c=0;

		diststr=c+3;
		c=strchr(diststr,'\n');
		*c=0;

		Dist = atoi(diststr);

		InsertCityList(&Cities, from);
		InsertCityList(&Cities, to);

		if(NumD >= MaxD) {
			void *v=realloc(D, (MaxD + 16) * sizeof(*D));
			if(!v) {
				printf("Error in realloc\n");
				return 1;
			}
			D=(Distance*)v;
			MaxD+=16;
		}

		memset(&D[NumD], 0, sizeof(*D));
		strncpy(D[NumD].Start, from, sizeof(D[NumD].Start));
		strncpy(D[NumD].End, to, sizeof(D[NumD].End));
		D[NumD].Dist = Dist;
		NumD++;

	}
	printf("Read %lu items\n", NumD);
	CreateTree(&Root, &v);
	b = DescendTree(&Root, 0);
	printf("Best route: %i\n", b->TotalDist);
	b = DescendTree(&Root, 1);
	printf("Worst route: %i\n", b->TotalDist);
	
	
	return 0;
}
