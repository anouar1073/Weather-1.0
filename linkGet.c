#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>


#define S_BUF_MAX 1000
#define S_BUF_MID 250
#define S_BUF_MID2 100
#define S_BUF_MIN 50

	int main(int argc, char ** argv) {
		
		/* remove these files if they exist */

                 if( access( "/tmp/lect" , F_OK ) != -1 ) {

    		// files exist, remove them

		int p = system("rm /tmp/lect /tmp/ecr");
		printf("%d   \n",p);
		system("rm /tmp/lect1 /tmp/ecr1");

		} 

		int fd_read,fd_write,fd_write_link;

		char * lect = "/tmp/lect";

		char * ecr = "/tmp/ecr";

		char * lect1 = "/tmp/lect1";

		char * ecr1 = "/tmp/ecr1";

		char * location_str, * weather_str1, * weather_str2, * weather_str3, * weather_str;

		char * link;

		char cmd[S_BUF_MID];

		location_str = ( char * ) malloc(sizeof(char) * S_BUF_MID);

		weather_str1 = (char * ) malloc(sizeof(char) * S_BUF_MID);

		weather_str2 = (char *) malloc(sizeof(char) * S_BUF_MIN);

		weather_str3 = (char *) malloc(sizeof(char) * S_BUF_MID);

		weather_str =  (char *) malloc(sizeof(char) * S_BUF_MID);

		link = (char * ) malloc(sizeof(char) * S_BUF_MID);

		location_str ="https://api.ipgeolocation.io/ipgeo?apiKey=cace341678194fed9de80768e609914d";

		weather_str1="api.openweathermap.org/data/2.5/weather?q=";

		weather_str3= "&lang=fr&units=metric&APPID=37d911c637a8e14ed6861dc3a05482ce";

		// create the pipes 

		if(mkfifo(lect,S_IRUSR | S_IWUSR) < 0)
			perror("echec de création du tube");
		if(mkfifo(ecr, S_IRUSR | S_IWUSR)<0)
			perror("echec de création du tube");
		if(mkfifo(lect1,S_IRUSR | S_IWUSR) < 0)
			perror("echec de création du tube");
		if(mkfifo(ecr1, S_IRUSR | S_IWUSR)<0)
			perror("echec de création du tube");


		fd_read=open(lect,O_RDONLY);
			if (fd_read<0)
				perror("failed to open pipe");
		fd_write=open(ecr,O_WRONLY);
			if (fd_write<0)
				perror("failed to open pipe");
		fd_write_link=open(lect1,O_WRONLY);
			if (fd_write_link<0)
				perror("failed to open pipe");


		while (1) {
		

		char option[S_BUF_MIN];
		
		read(fd_read,option,S_BUF_MIN);

			if( strcmp(option,"region")==0) {

				strcat(link,location_str);
			}

			else {

				memset(weather_str2,0,strlen(weather_str2));

				memset(weather_str,0,strlen(weather_str));

				strcat(weather_str2,option);

				strcat(weather_str,weather_str1);

				strcat(weather_str,weather_str2);

				strcat(weather_str,weather_str3);	

				strcat(link,weather_str);
			}
		

		write(fd_write_link,link,strlen(link)+1);

	        write(fd_write,"OK",3);

		memset(cmd,0,strlen(cmd));

		memset(weather_str2,0,strlen(weather_str2));

		memset(weather_str,0,strlen(weather_str));

		memset(link,0,strlen(link));

		}

		return 0;
	}
	
