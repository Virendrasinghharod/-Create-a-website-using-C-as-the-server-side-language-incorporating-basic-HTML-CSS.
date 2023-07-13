#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void sendResponse(int clientSocket, const std::string& response) {
    std::string httpResponse = "HTTP/1.1 200 OK\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Content-Length: " + std::to_string(response.length()) + "\r\n";
    httpResponse += "\r\n";
    httpResponse += response;

    send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Bind the server address to the socket
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if (listen(serverSocket, 5) == 0)
        std::cout << "Server started on port 8080..." << std::endl;
    else
        std::cout << "Failed to start the server!" << std::endl;

    addr_size = sizeof serverStorage;

    while (true) {
        // Accept client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);

        if (clientSocket >= 0) {
            // Receive client request
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            // Process the request
            std::string request(buffer);
            std::string response = "<html><head><title>Simple Web Server</title></head>";
            response += "<body><h1>Welcome to the Simple Web Server!</h1></body></html>";

            // Send the response
            sendResponse(clientSocket, response);
        }
    }

    return 0;
}
/*
As the code provided is a server-side code, it doesn't produce any direct output that you can see in the console. However, you can follow these steps to test the website:

Save the code in a file named webserver.cpp.
Compile the code using a C++ compiler. For example, if you're using g++, you can run the following command in your terminal:
Copy code
g++ -o webserver webserver.cpp
Run the compiled executable:
bash
Copy code
./webserver
If the server starts successfully, you should see the message Server started on port 8080... in the console.
Open a web browser and enter http://localhost:8080 in the address bar.
The browser should display the HTML response sent by the server, which includes the text "Welcome to the Simple Web Server!".
Please note that this basic web server can handle only one connection at a time, and it lacks error handling and advanced features. It's important to use proper web server frameworks or libraries for production-level web applications.
*/
