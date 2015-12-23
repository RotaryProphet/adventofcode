#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json.h>

int summ(json_object *obj) {
	int r=0;
	int i;

	switch(json_object_get_type(obj)) {
	case json_type_int:
		r = json_object_get_int(obj);
		break;
	case json_type_object:
	{
		json_object_object_foreach(obj, key0, val0) {
			/* Leave this here to remove the warning about unused key0 */
			json_object *val;
			json_object_object_get_ex(obj, key0, &val);
			if(
				json_object_get_type(val0) == json_type_string
			     && !strcmp(json_object_get_string(val0), "red")
			) {
				return 0;
			}
			r+=summ(val0);
		}
		break;
	}
	case json_type_array:
		for(i=0;i<json_object_array_length(obj);i++) {
			r+=summ(json_object_array_get_idx(obj, i));
		}
		break;
	default:
		break;
	}
	return r;	
}

int main(int argc, char *argv[]) {
	char *buffer=NULL, tmpbuf[1024+1];
	size_t bufsize=0;
	int total=0;
	int working=0;
	int neg=1;
	size_t i;
	size_t l;
	json_object *new_obj;

	while(fgets(tmpbuf, sizeof(tmpbuf)-1, stdin)) {
		tmpbuf[sizeof(tmpbuf)-1]=0;
		l = strlen(tmpbuf);

		buffer=(char*)realloc(buffer, bufsize + l + 1);
		memcpy(&buffer[bufsize], tmpbuf, l);
		buffer[bufsize+l]=0;
		bufsize+=l;
	}

	for(i=0;buffer[i];i++) {
		char c=buffer[i];

		if(c >= '0' && c <= '9') {
			working *= 10;
			working += c-'0';
		} else if(!working && c == '-') {
			neg=-1;
		} else {
			total+=working * neg;
			working=0;
			neg=1;
		}
	}
	printf("Day1: Total of numbers is %i\n", total);

	if(!(new_obj = json_tokener_parse(buffer))) {
		printf("Error parsing json...\n");
	}
	total = summ(new_obj);
	printf("Day2: Total of numbers is %i\n", total);

	return 0;
}
