#include <string>
class msgHandler
{
	private:
		std::string _msg;
	public:
		void sendWelcomeMessage(int client_fd, std::string nick);
		void handlerMessage(int client_fd, std::string nick);
		msgHandler(std::string msg);
		msgHandler();
		~msgHandler();	
};
