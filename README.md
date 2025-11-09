# Client-Servet-tasks
Projects where the client and server need to run simultaneously

server1, client1 | This basic task configures the connection and allows the client to type N(default = 5) ints and the server will check if the average >20    



# UNIX-server/client

This project consists of a server and a client implemented in C using UNIX-domain stream sockets. Multiple clients can connect and be served concurrently.  
The client reads a sequence of N integers from the user and sends it to the server.  
The server calculates the average of the received sequence:  
If the average is greater than 20, it returns the average and a success message (e.g., Sequence Ok).  
Otherwise, it returns a failure message (e.g., Check Failed).  
The client displays the server’s response and then asks the user whether to send another sequence.  
The communication ends when the user chooses to stop  
