#include"./myfunctions.c"

#define M_SIZE 40

#define SM_SIZE 20

#define S_SIZE 2

int main () {
		int i,fd,size,fd_read,fd_write,fd_read_json;

		char buffer[S_BUF_MAXXX],ready[S_SIZE];

		char option[M_SIZE],option1[SM_SIZE],option2[SM_SIZE];

		char city[M_SIZE],temp[M_SIZE];

		char location[M_SIZE];

		char * lect ="/tmp/lect";

		char * ecr = "/tmp/ecr";

		char * lect_json="/tmp/ecr1";


		fd_write=open(lect,O_WRONLY);

		fd_read=open(ecr, O_RDONLY);

		fd_read_json=open(lect_json,O_RDONLY);
	
		while(1) {

		menu_screen();

		label:

		scanf("%s",option);

		if (strcmp(option,"region")!=0 && strcmp(option,"other")!=0) {

				printf("Wrong options please enter either \"region\" or \"autre\" in lower case \n");

				goto label;
		}

		if(strcmp(option,"other")==0) {

			printf("insert name of city:");

			scanf(" %[^\n]s",option);

			memset(city,0,strlen(city));

			strcat(city,option);

		} else {

			memset(location,0,strlen(location));

			strcat(location,option);

		}

		write(fd_write,option,strlen(option)+1);

		read(fd_read,ready,3);

		if(strcmp(ready,"OK")==0) {

		size = read(fd_read_json,buffer,S_BUF_MAXXX);
		
		
		// inisialiser le parser

		jsmn_parser parser;

		jsmntok_t tokens[S_BUF_MAXX]; 

		jsmn_init(&parser);


		

		int numTok = jsmn_parse(&parser,buffer, strlen(buffer), tokens,sizeof(tokens)/sizeof(tokens[0]));

		// supposer que le tout premier element est un objet ( format JSON ) sinon - erreur

		if (numTok < 1 || tokens[0].type != JSMN_OBJECT) {

			printf("corrupted JSON data\n");

		}
		

		if(strcmp(location,"region")==0) {

			memset(city,0,strlen(city));

			memset(temp,0,strlen(temp));

			strcat(temp,keyFinder(buffer,tokens,"state_prov",numTok,size,"parent"));

			strcat(city,temp);

			// display infromation regardings the location of the user.
		
			display_location_info(buffer,tokens,numTok,size);


			write(fd_write,city,strlen(city)+1);

			read(fd_read,ready,3);

			memset(buffer,0,strlen(buffer));

			// wait for getLink process to generate the link and send it to the getHttp process

			if (strcmp(ready,"OK")==0) {

				printf("\n fetching ... \n");

				// read raw JSON from the pipe

				size = read(fd_read_json,buffer,S_BUF_MAXXX);

				jsmn_parser parser;

				jsmntok_t tokens[S_BUF_MAXX];

				jsmn_init(&parser);

				int numTok = jsmn_parse(&parser,buffer, strlen(buffer), tokens,sizeof(tokens)/sizeof(tokens[0]));

				if (numTok < 1 || tokens[0].type != JSMN_OBJECT) {

					printf("Corrupted JSON data\n");

					printf("%d \n" ,numTok);
			}
			// diplay city climate information
			
			display_city_climate_info(buffer,tokens,numTok,size,city);
			
		}

		}	
		
		 else {
			//  or display other location info

			display_city_climate_info(buffer,tokens,numTok,size,city);
			
		}
	}	
		memset(buffer,0,strlen(buffer));

		memset(option,0,strlen(option));

		memset(location,0,strlen(location));
		
	}
	 
		return 0;
	}

