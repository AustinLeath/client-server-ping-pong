THIS WAS DESIGNED TO RUN ON THE CSE SERVERS

1. Connect to any CSE server

2. Put these files on the server and change into the server sub-directory

3. Once in the server directory you can compile and run the server like so:
    - `gcc *.c`
    - `./a.out <port>`

4. Once the server is up and running, connect to the same CSE server in a different terminal and navigate to the client sub-directory

5. Once in the client directory you can compile and run the client like so:
    - `gcc *.c`
    - `./a.out <hostname> <port>`