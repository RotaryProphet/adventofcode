#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum _Ops {
	SET,
	AND,
	OR,
	LSHIFT,
	RSHIFT,
	NOT,
	_Ops_max
} Ops;

typedef struct _Node {
	char		*Target;
	bool		Factored;
	uint16_t	Value;
	char		*Statement;
	Ops		Operation;
	char		*A;
	char		*B;
	struct _Node	*Next;
} Node;

Node Keys;

Node *FindNode(const char *Name) {
	Node *n;

	for(n=Keys.Next; n; n=n->Next) {
		if(!strcmp(Name, n->Target)) {
			return n;
		}
	}
	return NULL;
}

uint16_t GetKeyValue(const char *Name) {
	Node *n;
	uint16_t a, b;
	
	if(Name[0] >= '0' && Name[0] <= '9') {
		return atoi(Name);
	}

	if(!(n=FindNode(Name))) {
		printf("Unable to find key '%s'\n", Name);
		exit(1);
	}

	if(n->Factored) {
		return n->Value;
	}

	a=GetKeyValue(n->A);
	if(n->B) {
		b=GetKeyValue(n->B);
	}

	switch(n->Operation) {
	case SET:
		n->Value=a;
		break;
	case AND:
		n->Value = a & b;
		break;
	case OR:
		n->Value = a | b;
		break;
	case LSHIFT:
		n->Value = a << b;
		break;
	case RSHIFT:
		n->Value = a >> b;
		break;
	case NOT:
		n->Value = ~a;
		break;
	case _Ops_max:
	default:
		printf("Invalid operation #%i\n", n->Operation);
		exit(1);
	}

	n->Factored=1;
	return n->Value;
}

int main(int argc, char *argv[]) {
	char buffer[128];
	uint16_t val;

	Node *n;

	memset(&Keys, 0, sizeof(Keys));

	while(fgets(buffer, sizeof(buffer), stdin)) {
		char *c;
		char *parts[10]={0};
		int i=0;

		if(!(n = (Node*)malloc(sizeof(*n)))) {
			printf("Error allocating node\n");
			exit(1);
		}
		memset(n, 0, sizeof(*n));

		if((c=strchr(buffer, '\n'))) {
			*c=0;
		}

		n->Statement = strdup(buffer);

		parts[i++] = buffer;
		while((c = strchr(parts[i-1], ' '))) {
			*c=0;
			parts[i++] = c+1;
		}
		
		if(i == 3) {
			n->Target = strdup(parts[2]);
			n->Operation = SET;
			n->A = strdup(parts[0]);
		} else if(i==4 && !strcmp(parts[0], "NOT")) {
			n->Target = strdup(parts[3]);
			n->Operation = NOT;
			n->A = strdup(parts[1]);
		} else if(i==5 && !strcmp(parts[1], "AND")) {
			n->Target = strdup(parts[4]);
			n->Operation = AND;
			n->A = strdup(parts[0]);
			n->B = strdup(parts[2]);
		} else if(i==5 && !strcmp(parts[1], "OR")) {
			n->Target = strdup(parts[4]);
			n->Operation = OR;
			n->A = strdup(parts[0]);
			n->B = strdup(parts[2]);
		} else if(i==5 && !strcmp(parts[1], "LSHIFT")) {
			n->Target = strdup(parts[4]);
			n->Operation = LSHIFT;
			n->A = strdup(parts[0]);
			n->B = strdup(parts[2]);
		} else if(i==5 && !strcmp(parts[1], "RSHIFT")) {
			n->Target = strdup(parts[4]);
			n->Operation = RSHIFT;
			n->A = strdup(parts[0]);
			n->B = strdup(parts[2]);
		} else {
			printf("Unknown operation: %s\n", n->Statement);
			exit(1);
		}
		n->Next = Keys.Next;
		Keys.Next = n;
	}

	val = GetKeyValue("a");
	printf("Value on 'a' is %u\n", val);

	for(n=Keys.Next;n;n=n->Next) {
		if(!strcmp(n->Target, "b")) {
			n->Value = val;
			n->Factored = 1;
		} else {
			n->Factored = 0;
		}
	}

	val = GetKeyValue("a");
	printf("Value on 'a' is %u\n", val);

	return 0;
}
