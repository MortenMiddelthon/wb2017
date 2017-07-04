#include <json-c/json.h>
#include <stdio.h>

int main() {
	FILE *json;
	char filename[] = "checkins.json";
	char json_string[255];
	json = fopen(filename, "r");
	while( fgets(json_string, sizeof(json_string), json) != NULL ) {
	//	printf("%s\n", json_string);
		json_object * jobj = json_tokener_parse(json_string);
		enum json_type type;
		json_object_object_foreach(jobj, key, val) {
			printf("type: ",type);
			type = json_object_get_type(val);
			switch (type) {
				case json_type_null: printf("json_type_null\n");
				break;
				case json_type_boolean: printf("json_type_boolean\n");
				break;
				case json_type_double: printf("json_type_double\n");
				break;
				case json_type_int: printf("json_type_int\n");
				break;
				case json_type_object: printf("json_type_object\n");
				break;
				case json_type_array: printf("json_type_array\n");
				break;
				case json_type_string: printf("json_type_string\n");
				break;
			}
		}
	}
	fclose(json);
}
