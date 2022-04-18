#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// run program via CMD in this form
// ./iniparser file_name.ini section.key

// SKV section key value, S section, KV keyvalue
struct SKV {
  char S[30];
  char KV[64];
};

// number of structures
int number = 0;

int main(int argc, char **argv){

    // splitting arg[2] into section and key
    char *section;
    char *key;
    section = strtok(argv[2], ".");
    key = strtok(NULL, ".");

    char sec[32]= "[";
    char bracket[1] = "]";
    strcat(sec, section);
    strncat(sec, bracket, 1);
    section = sec;

    FILE * fh;
	fh = fopen(argv[1], "r");

    // counting the number of structs needed
	if (fh != NULL)
	{
		char c[64];
		while (fgets(c, 64, fh) != NULL){
            // empty lines
            int x = strncmp("\n", c, strlen("\n"));
            // comments
            int y = strncmp(";", c, strlen(";"));
            // sections
            int z = strncmp("[", c, strlen("["));

            if (x!=0 && y!=0 && z!=0){
                number += 1;
            }
	}

    // when the file name is incorrect
	} else printf("Error opening the file.\n");

    // Memory allocation for n structures
    struct SKV *ptr;
    ptr = (struct SKV *)malloc(number * sizeof(struct SKV));

    // parsing the data into well defined structures
    // detecting invalid indentifiers
    // detecting missing sections and keys
	fh = fopen(argv[1], "r");
	if (fh != NULL)
	{
        //file lines
		char c[64];
        char h[64];
        char answer[32];
        int i = 0;
        // current section
        char CS [32];
        // missing secetion, missing key
        int MS = 1;
        int MK = 1;
		while (fgets(c, 64, fh) != NULL){
            strcpy(h, c);

            char *check;
            check = strtok(c, " = ");
            if (check[0] == '[') {
                while(*check !='\0') {
                    char chr = *(check++);
                    if (isalnum(chr) == 0 && chr != '-' && chr != '[' && chr != ']' && chr != '\n') {
                        printf("Invalid identifier: ");
                        printf(strtok(c, " = "));
                        break;
                    }
                }
            }
            else
                if (check[0] != ';') {
                    while (*check != '\0') {
                        char chr = *(check++);
                        if (isalnum(chr) == 0 && chr != '-' && chr != '\n') {
                            printf("Invalid identifier: ");
                            printf(strtok(c, " = "));
                            printf("\n");
                            break;
                        }
                    }
                }

            if (strncmp("[", h, strlen("[")) == 0) strcpy(CS, h);

            if (number > i && strncmp("\n", h, strlen("\n")) != 0 && strncmp(";", h, strlen(";")) != 0 && strncmp("[", h, strlen("[")) != 0){
                strcpy((ptr + i)->S, CS);
                strcpy((ptr + i)->KV, h);
                i += 1;

                if (0 == strncmp(CS, section, strlen(section))){
                    MS = 0;
                    char *file_key;
                    char *value;
                    file_key = strtok(h, " = ");
                    value = strtok(NULL, " = ");
                    if (strncmp(key, file_key, strlen(file_key))==0){
                        MK = 0;
                        strcpy(answer, value);
                    }
                }
            }
	}

	fclose(fh);

    if (MS == 0 && MK == 0) printf("\nValue: %s", answer);
    if (MS == 1) printf("\nMissing section: %s", section);
    if (MS == 0 && MK == 1) printf("\nMissing key: %s", key);

    free(ptr);
	
    }

    getchar();
    return 0;
}