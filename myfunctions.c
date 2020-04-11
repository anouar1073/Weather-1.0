#include"./myfunctions.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char * stringSub (char * s,int start, int end, int size) {
		char * result;
		result = malloc(size*sizeof(char)+1);
		for(int i=start; i<=end ; i++ ) {
			int j = i - start;
			*(result + j)= *(s+i);
		}
		*(result + end + 1) = '\0';
		return result;
}

char * keyFinder( char * json_str,jsmntok_t * tokens,char * s,int numTok,int size, char * object) {
			
		int i;
		char * result;
		int this_object=0;
                if( strcmp(object,"parent")==0)
                        this_object=1;

		
		for (i = 1; i < numTok; i++) {
			if( tokens[i].type==JSMN_OBJECT && jsoneq(json_str,&tokens[i],object)==0 )
                                this_object=1;

			if(jsoneq(json_str,&tokens[i], s)==0 && this_object==1) {
				result = stringSub(json_str,tokens[i+1].start, tokens[i+1].end-1,tokens[i+1].end - tokens[i+1].start-1);  
				result=strcat(result,"\0");
				return result;
		}
	}
		return "cannot find property - error";
	}

	char * objectFinder_array(char * json_str, jsmntok_t * tokens, char * s, int numTok, int size, int index) {
		int i;
		char * result;
		int object_num=0;
		for (i = 1; i<numTok;i++) {
			if( tokens[i].type==JSMN_OBJECT) {
				 char * key;
				key= stringSub(json_str,tokens[i+1].start, tokens[i+1].end-1, tokens[i+1].end - tokens[i+1].start-1);
				strcat(key,"\0");
				if(strcmp(key,"dt")==0) {
					if(object_num==index) {
					result=stringSub(json_str,tokens[i].start, tokens[i].end-1, tokens[i].end - tokens[i].start-1);
					return result;
				}
					else 
						object_num++;
			
			}
		}
	}
		return "cannot find element - error"; 
}

void menu_screen() {
			printf("\n \n ");
			printf("enter a city of your choice for weather information:\n");
			printf("\n");
			printf("1- city where I'm \n");
			printf("2- another city\n");
			printf("enter \"region\" for weather info in your region\n.");
			printf("enter\"other\" for weather info in another reagion of the world\n");
			printf("\n *******************************************************");
			printf("\n-> ");
			
	}

void display_location_info (char * json_str,jsmntok_t * tokens,int numTok, int size) {
			
			printf("\n");
			printf("longitude: ");
			printf("%s",keyFinder(json_str,tokens,"longitude",numTok,size,"parent"));
			printf("\n");
			printf("latitude: ");
			printf("%s",keyFinder(json_str,tokens,"latitude",numTok,size,"parent"));
			printf("\n");
			printf("Your ip address: %s\n",keyFinder(json_str,tokens,"ip",numTok,size,"parent"));
			printf("*********************************************************** \n");
		}


void display_city_climate_info(char *json_str,jsmntok_t * tokens, int numTok, int size,char* city) {
			printf("\n********************************************************\n");
			printf("weather informaiton in the region of %s:\n",city);
			printf("country code: %s ",keyFinder(json_str,tokens,"country",numTok,size,"parent"));
			printf("\n");
			printf("current weather:\n");
			printf("Temperature: %s C°",keyFinder(json_str,tokens,"temp",numTok,size,"parent"));
			printf("\n");
			printf("Description: %s",keyFinder(json_str,tokens,"description",numTok,size,"parent"));
			printf("\n");
			printf("Wind speed: %s km/h \n",keyFinder(json_str,tokens,"speed",numTok,size,"parent"));
			printf("Wind direction(incidence angle): %s°",keyFinder(json_str,tokens,"deg",numTok,size,"parent"));
			printf("\n");
			printf("Pressure: %s kpa",keyFinder(json_str,tokens,"pressure",numTok,size,"parent"));
			printf("\n");
			printf("humidity: %s%%",keyFinder(json_str,tokens,"humidity",numTok,size,"parent"));
			printf("\n");
			printf("************************************************************\n"); 
			}

void display_city_forecast(char *json_str,jsmntok_t * tokens, int numTok, int size) {
			int i;
			char day[S_BUF_MAX];
			printf(" la prévision pour 5 jours:_________________________________________\n");
			printf("			temp			humid			desc	                                    \n");
			for ( i =0; i< NUM_3H ;i+=8) {
			memset(day,0,strlen(day));
			strcat(day,objectFinder_array(json_str,tokens,"list",numTok,size,i));
			int size1= strlen(day);
			jsmn_parser parser1;
			jsmntok_t tokens1[S_BUF_MID];
			jsmn_init(&parser1);
			int numTok1= jsmn_parse(&parser1,day, strlen(day), tokens1,sizeof(tokens1)/sizeof(tokens1[0]));
			
			printf("%s : 	%s C°			%s%%			%s \n",keyFinder(day,tokens1,"dt_txt",numTok1,size1,"parent")
								 ,keyFinder(day,tokens1,"temp",numTok1,size1,"parent")
								 ,keyFinder(day,tokens1,"humidity",numTok1,size1,"parent")
								 ,keyFinder(day,tokens1,"description",numTok1,size1,"parent"));
		}
}

