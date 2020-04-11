#include <stdio.h>
#include "./curl/curl.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define ML_SIZE 200
#define XL_SIZE 20000

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	/*calculate the real size*/

	size_t realsize = size * nmemb;

	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
 	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
 
	mem->memory = ptr;

 	memcpy(&(mem->memory[mem->size]), contents, realsize);

  	mem->size += realsize;

  	mem->memory[mem->size] = 0;
 
	  return realsize;
}

int main() 
	
{

	char * lect1 = "/tmp/lect1";

	char * ecr1 = "/tmp/ecr1";

	int fd_read_link,fd_write_json, fd_dump_file;

	char link[ML_SIZE];

	fd_read_link = open(lect1,O_RDONLY);

	fd_write_json= open(ecr1,O_WRONLY);

	fd_dump_file= open("JSON", O_WRONLY | O_CREAT , 0644);

	while (1) {

	CURL *curl_handle;

	CURLcode res;

	struct MemoryStruct data;

	read(fd_read_link,link,ML_SIZE);
  	
	data.memory = malloc(1);  // we start with 1 Byte and add the rest when needed 
 
 	data.size = 0;     
 
  	curl_handle = curl_easy_init();

  	if(curl_handle) {
			 
 			 curl_easy_setopt(curl_handle, CURLOPT_URL, link);
 
  			
 			 curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  			// pass the received data to the callback

  			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&data);

 			//log curl_easy_perform() actions

			curl_easy_setopt(curl_handle,CURLOPT_VERBOSE,0);

  			 // optional, but some servers require user agent

  			curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 		
  			// perform the request

  			res = curl_easy_perform(curl_handle);
 
  			// check for errors

  			if(res != CURLE_OK) {

  			  fprintf(stderr, "curl_easy_perform() à échoué: %s\n",

      			      curl_easy_strerror(res));
  			}
  			else {
   				/*
   				 * data.memory points to a part of the memory that contains the file of size data.size Bytes
    			  	 */ 
				//printf("%s \n",data.memory);

				char buffer_str[XL_SIZE];

				memset(buffer_str,0,strlen(buffer_str));

				memcpy(buffer_str,data.memory,strlen(data.memory));

				buffer_str[data.size]='\0';

				// send the data to the main process

				write(fd_write_json,buffer_str,data.size+1);

				// store the data in a file for debugging pursoses

				write(fd_dump_file,buffer_str,data.size+1);

   				printf("%lu Bytes received data \n", (unsigned long)data.size);

 			 }
 
   
  			curl_easy_cleanup(curl_handle);
 
  			free(data.memory);
 
  			curl_global_cleanup();
  			
			}

			memset(link,0,strlen(link));
		}

  	return 0;
}

