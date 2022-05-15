#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <errno.h>


//the thread function
void *connection_handler(void *);
//void get_broadcast_ip(char* ip_mask);

char *binary_to_int(char* binary){
  
  int res = (int) strtol(binary, NULL, 2);
  char *str = calloc(12, sizeof(char));
  sprintf(str, "%d", res);
  return str;

}

int invertirBits(int num)
{
	int invertido = 0;
	int i;
	for (i = 0; i < 8; ++i) {
		if(num & (1 << i)){
    		invertido |= 0 << i;
		}else{
    		invertido |= 1 << i;
		}
	}

	return invertido;
}

char *from_bits_to_doted(char* mask){

	char res[5];
	int bits = 0, x = 0, y = 0;

	char *doted = calloc(40, sizeof(char));
	char *partial = calloc(10, sizeof(char));
	strcpy(res, mask);
	memmove(res, res+1, strlen(res));
	bits = atoi(res);

	while(x < 33){
		
		if((y == 8) & (x < 32)){
			strcat(doted, binary_to_int(partial));
			strcat(doted, ".");
			y = 0;
			partial = calloc(10, sizeof(char));
		
		}else if((y == 8) & (x < 33)){
			strcat(doted, binary_to_int(partial));
			y = 0;
			partial = calloc(10, sizeof(char));
		
		}else if(bits > 0){
			strcat(partial, "1");
			bits--;
			x++;
			y++;
		
		}else{
			strcat(partial, "0");
			bits--;
			x++;
			y++;
		}
	}

	return doted;

}

char *get_broadcast_ip(char* ip, char* mask) {

	if(strlen(mask) < 8){
		mask = from_bits_to_doted(mask);
	}

	char **ip_parts = malloc(4 * sizeof(char*));
	char **mask_parts = calloc(4, sizeof(char*));
	int ip_piv = 0;
	int mask_piv = 0;
	int piv = 0;

	char *substr_ip = strtok(ip, ".");
	ip_parts[0] = malloc(3 * sizeof(char));
  	strcpy(ip_parts[0], substr_ip);
	int x = 1;
	while (x < 4){
		substr_ip = strtok(NULL, ".");
		ip_parts[x] = malloc((3) * sizeof(char));
  		strcpy(ip_parts[x], substr_ip);
		x++;
	}

	char *substr_mask = strtok(mask, ".");
	mask_parts[0] = calloc(3, sizeof(char));
	strcpy(mask_parts[0], substr_mask);
  	
	x = 1;
	while (x < 4){
		substr_mask = strtok(NULL, ".");
		mask_parts[x] = malloc((3) * sizeof(char));
  		strcpy(mask_parts[x], substr_mask);
		x++;
	}
	char *res = calloc(20, sizeof(char));
	char str[12];

	while(piv < 4){

		ip_piv = atoi(ip_parts[piv]);
		mask_piv = atoi(mask_parts[piv]);

		if(piv == 0){
			sprintf(str, "%d", ip_piv|invertirBits(mask_piv));
			strcpy(res, str);
		
		}else{
			sprintf(str, "%d", ip_piv|invertirBits(mask_piv));
			strcat(res, ".");
			strcat(res, str);
		}

		piv++;
	}

	perror("Message from perror");
	return strcat(res, "\n");

}

char *get_net_num_ip(char* ip, char* mask) {

	if(strlen(mask) < 8){
		mask = from_bits_to_doted(mask);
	}

	char **ip_parts = malloc(4 * sizeof(char*));
	char **mask_parts = calloc(4, sizeof(char*));
	int ip_piv = 0;
	int mask_piv = 0;
	int piv = 0;

	char *substr_ip = strtok(ip, ".");
	ip_parts[0] = malloc(3 * sizeof(char));
  	strcpy(ip_parts[0], substr_ip);
	int x = 1;
	while (x < 4){
		substr_ip = strtok(NULL, ".");
		ip_parts[x] = malloc((3) * sizeof(char));
  		strcpy(ip_parts[x], substr_ip);
		x++;
	}

	char *substr_mask = strtok(mask, ".");
	mask_parts[0] = calloc(3, sizeof(char));
	strcpy(mask_parts[0], substr_mask);
  	
	x = 1;
	while (x < 4){
		substr_mask = strtok(NULL, ".");
		mask_parts[x] = malloc((3) * sizeof(char));
  		strcpy(mask_parts[x], substr_mask);
		x++;
	}
	char *res = calloc(20, sizeof(char));
	char str[12];

	while(piv < 4){

		ip_piv = atoi(ip_parts[piv]);
		mask_piv = atoi(mask_parts[piv]);

		if(piv == 0){
			sprintf(str, "%d", ip_piv&mask_piv);
			strcpy(res, str);
		
		}else{
			sprintf(str, "%d", ip_piv&mask_piv);
			strcat(res, ".");
			strcat(res, str);
		}

		piv++;
	}

	perror("Message from perror");
	return strcat(res, "\n");

}

char *get_host_range(char* ip, char* mask){

	if(strlen(mask) < 8){
		mask = from_bits_to_doted(mask);
	}

	char *ip1 = calloc(20, sizeof(char));
	char *ip2 = calloc(20, sizeof(char));
	char *mask1 = calloc(20, sizeof(char));
	char *mask2 = calloc(20, sizeof(char));

	strcpy(ip1, ip);
	strcpy(ip2, ip);
	strcpy(mask1, mask);
	strcpy(mask2, mask);

	char *start = get_net_num_ip(ip1, mask1);
	char *end = get_broadcast_ip(ip2, mask2);

	char *res = calloc(20, sizeof(char));
	char str[12];

	char *substr_start = strtok(start, ".");

	strcpy(res, substr_start);

	int x = 1, y = 0, p = 0;
	while (x < 4){
		printf("Start: %s \n", res);
		if(x == 3){
			strcat(res, ".");
			substr_start = strtok(NULL, ".");
			printf("Start: %s \n", substr_start);
			y = atoi(substr_start) + 1;
			sprintf(str, "%d", y);
			strcat(res, "{");
			strcat(res, str);
			strcat(res, "-");

		}else{
			strcat(res, ".");
			substr_start = strtok(NULL, ".");
			strcat(res, substr_start);
		}

		x++;
	}

	char *substr_end = strtok(end, ".");
	x = 0;
	while (x < 4){

		if(x == 3){
			p = atoi(substr_end) - 1;
			sprintf(str, "%d", p);
			strcat(res, str);
			strcat(res, "}");

		}else{
			substr_end = strtok(NULL, ".");
		}

		x++;
	}

	return strcat(res, "\n");

}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9666);
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
        if(strncmp(client_message, "GET BROADCAST IP", 16) == 0){

			char *substr = strtok(client_message, " ");
			char *ip;
			char *mask;
			int flag = 0;
			int x = 0;
			while (x < 5)
			{
				
				substr = strtok(NULL, " ");
				
				if(x == 2){
					ip = substr;
				
				}else if(x == 3){
					if(strcmp(substr, "MASK")){
						flag = 1;
					}

				}else if(x == 4){
					mask = substr;
				}
				
				x++;
			}

			if(flag == 1){
				message = "Formato de comando invalido";
				write(sock, message, strlen(message));

			}else{
				message = get_broadcast_ip(ip, mask);
				write(sock, message, strlen(message));

			}
			
            
        }

		else if(strncmp(client_message, "GET NETWORK NUMBER IP", 21) == 0){

			char *substr = strtok(client_message, " ");
			char *ip;
			char *mask;
			int flag = 0;
			int x = 0;
			while (x < 6)
			{
				//printf("'%s'\n", substr);
				substr = strtok(NULL, " ");
				printf("'%s'\n", substr);
				printf("%d \n", x);
				if(x == 3){
					ip = substr;
				
				}else if(x == 4){
					if(strcmp(substr, "MASK")){
						flag = 1;
					}

				}else if(x == 5){
					mask = substr;
				}
				
				x++;
			}

			if(flag == 1){
				message = "Formato de comando invalido";
				write(sock, message, strlen(message));

			}else{
				message = get_net_num_ip(ip, mask);
				write(sock, message, strlen(message));
				
			}
			
            
        }else if(strncmp(client_message, "GET HOSTS RANGE IP", 18) == 0){

			char *substr = strtok(client_message, " ");
			char *ip;
			char *mask;
			int flag = 0;
			int x = 0;
			while (x < 6)
			{
				//printf("'%s'\n", substr);
				substr = strtok(NULL, " ");
				printf("'%s'\n", substr);
				printf("%d \n", x);
				if(x == 3){
					ip = substr;
				
				}else if(x == 4){
					if(strcmp(substr, "MASK")){
						flag = 1;
					}

				}else if(x == 5){
					mask = substr;
				}
				
				x++;
			}

			if(flag == 1){
				message = "Formato de comando invalido";
				write(sock, message, strlen(message));

			}else{
				message = get_host_range(ip, mask);
				write(sock, message, strlen(message));
				
			}
			
            
        }
	}
	
	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}