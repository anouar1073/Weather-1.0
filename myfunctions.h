#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include"jsmn/jsmn.h"
#include"jsmn/jsmn.c"

#define S_BUF_MAXXX 20000
#define S_BUF_MAXX 3000
#define S_BUF_MAX 1000
#define S_BUF_MIDD 500
#define S_BUF_MID 250
#define S_BUF_MID2 100
#define S_BUF_MIN 50
#define NUM_3H 40

static int jsoneq(const char *json, jsmntok_t *tok, const char *s);

char * stringSub (char * s,int start, int end, int size);

char * keyFinder( char * json_str,jsmntok_t * tokens,char * s,int numTok,int size, char * object);

char * keyFinder_array(char * json_str, jsmntok_t * tokens, char * s, int numTok, int size, int index);

void menu_screen();

void display_location_info (char * json_str,jsmntok_t * tokens,int numTok, int size);

void display_city_climate_info(char *json_str,jsmntok_t * tokens, int numTok, int size,char* city);

