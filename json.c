#include <json-c/json.h>
#include <stdio.h>

void json_parse(json_object * jobj);

int main() {
	FILE *jsonInput;
	int counter = 0;
	size_t len = 0;
	char filename[] = "checkins.json";
	char *jsonString = NULL;
	jsonInput = fopen(filename, "r");
	while( getline(&jsonString, &len, jsonInput) != -1 ) {
	//	printf("%s\n", json_string);
		json_object * jobj = json_tokener_parse(jsonString);
		counter++;
		if(jobj != NULL) {
			json_parse(jobj);
		}
		else {
			printf("Problem at line %d\n", counter);
			printf("#######\n");
			printf("%s\n", jsonString);
			printf("#######\n");
		}
		/*
		json_object_object_foreach(jobj, key, val) {
		}
		*/
	}
	fclose(jsonInput);
}

void json_parse(json_object * jobj) {
	enum json_type type;
	json_object_object_foreach(jobj, key, val) {
		type = json_object_get_type(val);
		switch (type) {
			case json_type_string: printf("key: %s, ", key);
			printf("value: %s\n", json_object_get_string(val));
			break;
		}
	}
}
