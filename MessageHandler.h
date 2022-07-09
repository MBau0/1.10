#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <vector>
#include <memory>

struct Message;

class Client;
class EntityManager;

class MessageHandler {
public:
	MessageHandler(Client* client, EntityManager* entity_manager);

	void process();


private:
	std::vector<std::shared_ptr<Message>> _messages;

	EntityManager* _entity_manager;
};

#endif
