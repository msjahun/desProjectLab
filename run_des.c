
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>

#define SA struct sockaddr
#define LISTENQ 5

//for reading and displaying file
#define BUFFER_SIZE 20


/*
 * des.h provides the following functions and constants:
 *
 * generate_key, generate_sub_keys, process_message, ENCRYPTION_MODE, DECRYPTION_MODE
 *
 */
#include "des.h"

// Declare file handlers
static FILE *key_file, *input_file, *output_file;

// Declare action parameters
#define ACTION_GENERATE_KEY "-g"
#define ACTION_ENCRYPT "-e"
#define ACTION_DECRYPT "-d"

// DES key is 8 bytes long
#define DES_KEY_SIZE 8

//function prototypes and functions
int Des_function(int argc, char *argv[]);

int read_line(char str[], int n){
	int ch, i = 0;

	while((ch = getchar()) != '\n')
	if (i<n)
	str[i++] = ch;
	str[i] = '\0'; /*terminates strings */
	return i; /* number of characters stored */

}

void clearbuff(char buff[])
{
	int i = 0;
	char *b;
	b = buff;
	for (; i < BUFFER_SIZE; i++)
	{
		*b = '\0';
		b++;
	}
}

int howManyChars(char buff[])
{
	int counter = 0;
	char *b;
	b = buff;

	while (*b != '\0')
	{
		b++;
		counter++;
	}

	return counter;
}

int SendFileViaNetwork(int argc, char **argv)
{
	//client invoker

	int sockfd;
	char fname[25];
	int len, count;
	struct sockaddr_in servaddr, cliaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
	char buffer[BUFFER_SIZE];

	int fdold;

	fdold = open(argv[2], O_RDONLY);
	if (fdold == -1)
	{
		printf("Server could find file %s\n", buffer);
		send(sockfd, "error", 5, 0);
	}
	else
	{
		printf("\nFile sending in progress.");
		while ((count = read(fdold, buffer, BUFFER_SIZE - 1)) > 0)
		{
			send(sockfd, buffer, count + 1, 0);
			//printf("%s",buf);
			printf(".");

			//write(fdnew, buffer, count);
			clearbuff(buffer);
		}
		//fscanf(f,"%s",buffer);
		//printf("%s", buffer);
		write(sockfd, buffer, BUFFER_SIZE);

		printf("the file was sent successfully");
		close(sockfd);
	}

	return 0;
}

int ReceiveFileViaNetwork(int argc, char **argv)
{
	//server listener

	int fd, sockfd, listenfd, connfd;
	pid_t childpid;
	socklen_t client;
	struct sockaddr_in servaddr, cliaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
	client = sizeof(cliaddr);
	connfd = accept(listenfd, (SA *)&cliaddr, &client);
	char buffer[BUFFER_SIZE];
	int fdnew, n;
	//read(connfd,buffer,BUFFER_SIZE);

	printf("File received Successfully, getting ready to initiate download..\n");
	sleep(1);
	fdnew = creat(argv[2], 0666);
	if (fdnew == -1)
	{
		printf("cannot create file %s\n", argv[2]);
		exit(1);
	}
	clearbuff(buffer);
	/* Receiving and printing a reply from server */
	printf("\n\nReceived Data:\n");
	while (n = recv(connfd, buffer, sizeof(buffer), 0) > 0)
	{
			printf("%s",buffer);
		write(fdnew, buffer, howManyChars(buffer));
		clearbuff(buffer);
	}
printf("\nEnd of Data received\n\n");
    close(connfd);
	
	printf("File transfer complete TCP CLIENT terminated...\n");

	printf("the file was received successfully\n");
	
	return 0;
}

//here this function content
int CreateWriteDataToFile(char *argv)
{
/* Variable to store user content */
char data[1024];
	/* File pointer to hold reference to our file */
	FILE *fPtr;

	/* 
     * Open file in w (write) mode. 
     * "data/file1.txt" is complete path to create file
     */
	fPtr = fopen(argv, "w");

	/* fopen() return NULL if last operation was unsuccessful */
	if (fPtr == NULL)
	{
		/* File not created hence exit */
		printf("Unable to create file.\n");
		exit(EXIT_FAILURE);
	}

	/* Input contents from user to store in file */
	printf("Enter contents to store in file : ");
	scanf("%s", data);
/* Write data to file */
printf("Input is %s: ", data);

	/* Write data to file */
	fputs(data, fPtr);

	/* Close file to save file data */
	fclose(fPtr);

	/* Success message */
	printf("File created and saved successfully.\n");

return 0;
	
}



int DisplayFileContents(char *argv)
{
	int file;
	char buffer[BUFFER_SIZE];
	int read_size;
	file = open(argv, O_RDONLY);

	if (file == -1)
	{
		fprintf(stderr, "Error: %s: file not found\n", argv);
		return (-1);
	}
	while ((read_size = read(file, buffer, BUFFER_SIZE)) > 0)
		write(1, &buffer, read_size);

	close(file);
	return 0;
}



void Receive_encrypted_file()
{
	printf("Receive encrypted file function");
	char *argx[3];
	argx[1] = "700";
	argx[2] = "recievedFile";
	ReceiveFileViaNetwork(3, argx);
	printf("\n");

	//display received and decrypted file
	char keyfile[256];

	printf("\nPlease enter the name of keyfile use to decrypt Message: ");
	scanf("%s", keyfile);

	//it should decrypt the file here
char *argk[5];
int status;
	argk[1] = "-d";

	argk[2] = keyfile;
	argk[3] = argx[2];
	argk[4] = "receivedFileDecrypted";
	//run_des [-e|-d] keyfile.key input.file output.file
	status = Des_function(5, argk);

printf("\nPrinting Decrypted text\n");
	DisplayFileContents("receivedFileDecrypted");
}



void Create_and_send_encrypted_file()
{
	printf("Create and send encrypted file function");
	char *nameofFileEncrypted = "EncryptedFile";
	char *nameofFileUnEncrypted = "fileToEncrypt";

CreateWriteDataToFile(nameofFileUnEncrypted);
	//There is an issue with this line, doesn't wait for user input
	//CreateWriteDataToFile(nameofFileUnEncrypted, "Hello world data");
	//encrypt and send over network
	int status;
	char keyfile[256];

	printf("\nPlease enter the name of keyfile use to encrypt Message: ");
	scanf("%s", keyfile);
	char *argv[5];
	argv[1] = "-e";

	argv[2] = keyfile;
	argv[3] = nameofFileUnEncrypted;
	argv[4] = nameofFileEncrypted;
	//run_des [-e|-d] keyfile.key input.file output.file
	status = Des_function(5, argv);

printf("Finished encryption with status %d", status);

	//send file over network
	char *argx[3];
	argx[1] = "700";
	//argx[2] = nameofFileUnEncrypted; //sends unencrypted file
	argx[2] = nameofFileEncrypted; //sends encrypted file
	SendFileViaNetwork(3, argx);
	printf("\n");
}



void GenerateKey()
{
	int status;
	char keyfile[256];

	printf("Generate Key function\n");
	printf("\nPlease enter the name of keyfile you want to generate: ");
	scanf("%s", keyfile);
	char *argv[3];
	argv[1] = "-g";

	argv[2] = keyfile;

	status = Des_function(3, argv);
	printf("\n Des key has been generated Successfully\n");
}


void SendEncryptedKeyOverNetwork()
{
	printf("Send encrypted key over the network\n");
	//file sender client
	printf("please enter key name to send over the network: ");
	//scanf the keyname and assign it to argv[2];
	char keyfile[256];

	char *argv[3];
	argv[1] = "700";
	scanf("%s", keyfile);
	argv[2] = keyfile;
	SendFileViaNetwork(3, argv);
	printf("\n");
}



void ReceiveEncryptedKeyOverNetwork()
{
	printf("Receive encrypted key over network\n");
	printf("please enter key name to save for future use: ");
	//scanf the keyname and assign it to argv[2];
	//file receiver server
	char keyfile[256];

	char *argv[3];
	argv[1] = "700";
	scanf("%s", keyfile);
	argv[2] = keyfile;


	ReceiveFileViaNetwork(3, argv);
//receive file, decrypt it 


//display it.

	printf("\n");
}



int main(int argc, char *argv[])
{	char sen[500];
	char option, exitOption;

	printf("************************************************\n");
	printf("************************************************\n");
	printf("***********DES Program with full end to end ****\n");
	printf("************ Network encryption ****************\n");
	printf("************************************************\n\n");

optionSelection:
	printf("\n\n   Welcome Please select the following options\n");
	printf("\t1. Recive encrypted file, decrypt and display it\n");
	printf("\t2. Created a file encrypt it and send it to the receiving party\n");
	printf("\t3. Generate encryption key\n");
	printf("\t4. Send encryption key over the network\n");
	printf("\t5. Receive encryption key\n");
	printf("\t6. Display a file, same function with ./cat\n");
	printf("\t7. Create and write to file\n");
	printf("\t8. Test file receiver server\n");
	printf("\t9. Test file sender server\n");
	printf("\t or type q or Q to exit\n");

readOptionAgain:
	scanf("%c", &option);

	switch (option)
	{
	case '1':
	{
		Receive_encrypted_file();
		break;
	}
	case '2':
	{
		Create_and_send_encrypted_file();
		break;
	}
	case '3':
	{
		GenerateKey();
		break;
	}
	case '4':
	{
		SendEncryptedKeyOverNetwork();
		break;
	}
	case '5':
	{
		ReceiveEncryptedKeyOverNetwork();
		break;
	}
	case '6':
	{
		char nameofFile[256];
		printf("\nEnter file name you want to display: ");
		scanf("%s", nameofFile);
		DisplayFileContents(nameofFile);
		printf("\n");
		break;
	}

	case '7':
	{
		char nameofFile[256];
		char data[1024];
		printf("\nEnter file name you want to Create and write to: ");
		scanf("%s", nameofFile);

		CreateWriteDataToFile(nameofFile);
		printf("\n");
		break;
	}

	case '8':
	{
		//file receiver server
		char *argv[3];
		argv[1] = "700";
		argv[2] = "filenameReceived";
		ReceiveFileViaNetwork(3, argv);
		printf("\n");
		break;
	}

	case '9':
	{
		//file sender client
		char *argv[3];
		argv[1] = "700";
		argv[2] = "add.txt";
		SendFileViaNetwork(3, argv);
		printf("\n");
		break;
	}
	case 'q':
	case 'Q':
	{
		printf("\nProgram will exit now");
		return 0;
	}
	default:
	{
		printf("\n You have selected a wrong option please try again\nEnter option again: ");
		goto readOptionAgain;
	}
	}

	printf("\n\n Operation complete do you want to exit program or reselect option\n Press C or c to Continue or any key to exit: ");

	scanf("%c", &exitOption);
	scanf("%c", &exitOption);
	if (exitOption == 'C' || exitOption == 'c')
		goto optionSelection;

	//else
	printf("\n\nExiting... \n\n\n");
	return 0;
}

int Des_function(int argc, char *argv[])
{
	clock_t start, finish;
	double time_taken;
	unsigned long file_size;
	unsigned short int padding;

	if (argc < 2)
	{
		printf("You must provide at least 1 parameter, where you specify the action.");
		return 1;
	}

	if (strcmp(argv[1], ACTION_GENERATE_KEY) == 0)
	{ // Generate key file
		if (argc != 3)
		{
			printf("Invalid # of parameter specified. Usage: run_des -g keyfile.key");
			return 1;
		}

		key_file = fopen(argv[2], "wb");
		if (!key_file)
		{
			printf("Could not open file to write key.");
			return 1;
		}

		unsigned int iseed = (unsigned int)time(NULL);
		srand(iseed);

		short int bytes_written;
		unsigned char *des_key = (unsigned char *)malloc(8 * sizeof(char));
		generate_key(des_key);
		bytes_written = fwrite(des_key, 1, DES_KEY_SIZE, key_file);
		if (bytes_written != DES_KEY_SIZE)
		{
			printf("Error writing key to output file.");
			fclose(key_file);
			free(des_key);
			return 1;
		}

		free(des_key);
		fclose(key_file);
	}
	else if ((strcmp(argv[1], ACTION_ENCRYPT) == 0) || (strcmp(argv[1], ACTION_DECRYPT) == 0))
	{ // Encrypt or decrypt
		if (argc != 5)
		{
			printf("Invalid # of parameters (%d) specified. Usage: run_des [-e|-d] keyfile.key input.file output.file", argc);
			return 1;
		}

		// Read key file
		key_file = fopen(argv[2], "rb");
		if (!key_file)
		{
			printf("Could not open key file to read key.");
			return 1;
		}

		short int bytes_read;
		unsigned char *des_key = (unsigned char *)malloc(8 * sizeof(char));
		bytes_read = fread(des_key, sizeof(unsigned char), DES_KEY_SIZE, key_file);
		if (bytes_read != DES_KEY_SIZE)
		{
			printf("Key read from key file does nto have valid key size.");
			fclose(key_file);
			return 1;
		}
		fclose(key_file);

		// Open input file
		input_file = fopen(argv[3], "rb");
		if (!input_file)
		{
			printf("Could not open input file to read data.");
			return 1;
		}

		// Open output file
		output_file = fopen(argv[4], "wb");
		if (!output_file)
		{
			printf("Could not open output file to write data.");
			return 1;
		}

		// Generate DES key set
		short int bytes_written, process_mode;
		unsigned long block_count = 0, number_of_blocks;
		unsigned char *data_block = (unsigned char *)malloc(8 * sizeof(char));
		unsigned char *processed_block = (unsigned char *)malloc(8 * sizeof(char));
		key_set *key_sets = (key_set *)malloc(17 * sizeof(key_set));

		start = clock();
		generate_sub_keys(des_key, key_sets);
		finish = clock();
		time_taken = (double)(finish - start) / (double)CLOCKS_PER_SEC;

		// Determine process mode
		if (strcmp(argv[1], ACTION_ENCRYPT) == 0)
		{
			process_mode = ENCRYPTION_MODE;
			printf("Encrypting..\n");
		}
		else
		{
			process_mode = DECRYPTION_MODE;
			printf("Decrypting..\n");
		}

		// Get number of blocks in the file
		fseek(input_file, 0L, SEEK_END);
		file_size = ftell(input_file);

		fseek(input_file, 0L, SEEK_SET);
		number_of_blocks = file_size / 8 + ((file_size % 8) ? 1 : 0);

		start = clock();

		// Start reading input file, process and write to output file
		while (fread(data_block, 1, 8, input_file))
		{
			block_count++;
			if (block_count == number_of_blocks)
			{
				if (process_mode == ENCRYPTION_MODE)
				{
					padding = 8 - file_size % 8;
					if (padding < 8)
					{ // Fill empty data block bytes with padding
						memset((data_block + 8 - padding), (unsigned char)padding, padding);
					}

					process_message(data_block, processed_block, key_sets, process_mode);
					bytes_written = fwrite(processed_block, 1, 8, output_file);

					if (padding == 8)
					{ // Write an extra block for padding
						memset(data_block, (unsigned char)padding, 8);
						process_message(data_block, processed_block, key_sets, process_mode);
						bytes_written = fwrite(processed_block, 1, 8, output_file);
					}
				}
				else
				{
					process_message(data_block, processed_block, key_sets, process_mode);
					padding = processed_block[7];

					if (padding < 8)
					{
						bytes_written = fwrite(processed_block, 1, 8 - padding, output_file);
					}
				}
			}
			else
			{
				process_message(data_block, processed_block, key_sets, process_mode);
				bytes_written = fwrite(processed_block, 1, 8, output_file);
			}
			memset(data_block, 0, 8);
		}

		finish = clock();

		// Free up memory
		free(des_key);
		free(data_block);
		free(processed_block);
		fclose(input_file);
		fclose(output_file);

		// Provide feedback
		time_taken = (double)(finish - start) / (double)CLOCKS_PER_SEC;
		printf("Finished processing %s. Time taken: %lf seconds.\n", argv[3], time_taken);
		return 0;
	}
	else
	{
		printf("Invalid action: %s. First parameter must be [ -g | -e | -d ].", argv[1]);
		return 1;
	}

	return 0;
}
