
[![GCC](https://img.shields.io/badge/C%20-Programming-brightgreen.svg)](https://github.com/msjahun/desProjectLab)
[![GCC](https://img.shields.io/badge/DES-Encryption-red.svg)](https://github.com/msjahun/desProjectLab)
[![GCC](https://img.shields.io/badge/gcc-compiler-orange.svg)](https://github.com/msjahun/desProjectLab)
<h3>DES messaging/file transfer service using with DES encryption Baked in </h3>
The program can act as both client and server and since we’re using sockets communication is bi-directional, 
it Compile run_des.o that will be used for encrypted and decrypt
 <br>gcc  des.c run_des.c -o run_des
 
run the program from  a linux terminal
<br>./run_des

When the program is launched You get 9 options
1.	Receive encrypted file, decrypt and display it
2.	Create a file, encrypt it and sent it to the receiving party
3.	Genearate encryption key
4.	Send encryption key over the network
5.	Receive encryption key
6.	Display a file, same function with ./cat
7.	Create and write to file
8.	Test file receiver server
9.	Test file sender server
Or type q or Q to exit.

The following is the detailed explanation of each of the functions

1. Receive encrypted file, decrypt and display it
	When this option is chosen they program acts as a sever and wait for file to be sent over the network, it listens to a socket on a specific port and the Host that sends the file sends it through the port.

2. Create a file, encrypt it and sent it to the receiving party
	This asks the user for input and the key file to be used for the encryption after that a file is created with the user input and it is then encrypted and sent to the other receiving host.

3. Genearate encryption key
	This generates encryption key for the user and when they key is generated is can be used for encryption and decryption of files, also this key that is generated here can be sent over the network to another computer or copied through flash to use on another host.

4. Send encryption key over the network
	This option allows a user to select a key to send over the network, the host on the other side of the network has to listen for the encryption key before he can successfully receive it.

5. Receive encryption key
	This option allows a user to listen via a socket for a key to be sent. The host can then use the key to decrypt files that are encrypted with the key.

6. Display a file, same function with ./cat
	This option is put inplace to test functionality of the program, this option is used for displaying file contents, it has the same functionality with cat program in linux

7. Create and write to file
	This method is used for testing Creating a message and writing it to a file. The program uses this function to create a message by taking users input and saving it to a file, the file is then encrypted and sent over the network later on.

8. Test file receiver server
	This option is used for testing functionlity, it is used to test the networking function, it used to listen for a test file. It can be used to test whether the file transfer networking functionality if working.

9. Test file sender server
	This option is used for testing functinality too, it sends a sample file add.txt over the network with encrypting it, It can only be used when the second host computer is litening and waiting to receive the file over the network.
