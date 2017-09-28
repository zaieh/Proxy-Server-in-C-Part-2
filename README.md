# Proxy-Server-in-C-Part-2
Updates: if 200 OK cached, otherwise simply sent to client. Look at code. 

    --- A list.txt is attached, as well as records of some HTTP Requests made. 
//Additions: Once the http request is received in the server, the server checks whether the 
//http code is 200 if it is then the html page is cached intoa a file added to the list of cached
//pages with its respective file name, and finally sent to the client. 
//Else if the code isn't 200 OK then the html page is directly sent to the client without further action.
//At the end of either or options above, the server waits for the client to request yet another url and 
//proceed in the same manner. This time , however, the url is first searched in the list of cached websites,
//and if found simply given to the client as is.
