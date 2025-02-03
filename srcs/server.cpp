#include "../includes/lib.hpp"
/* 	public :
		server(std::string port, std::string password);
		~server();
		//todo
		std::string getPort(void);
		std::string getPass(void);
		void listen();
		void bind();
		void accept();
		void read();
		void send();
*/

void server::bindSocket()
{
	int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) 
	{
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
	
	address.sin_port = htons(std::stoi(this->getPort()));

    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
	{
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

}
void server::listenClient(){}
void server::accept(){}
void server::read(){}
void server::send(){}
