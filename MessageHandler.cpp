#include "MessageHandler.h"

#include "Message.h"
#include "TransformMessage.h"
#include "EntityMessage.h"

#include "EntityManager.h"
#include "Client.h"

MessageHandler::MessageHandler(Client* client, EntityManager* entity_manager) :
	_entity_manager			( entity_manager )
{
	client->set_message_receiver(&_messages);
}

void MessageHandler::process() {
	for (auto message : _messages) {
		switch (message->type()) {
		case GAME_TRANSFORM_MESSAGE:
			_entity_manager->transform_message(std::static_pointer_cast<TransformMessage>(message));
			break;
		case GAME_ENTITY_MESSAGE:
			_entity_manager->entity_message(std::static_pointer_cast<EntityMessage>(message));
			break;
		default:
			break;
		}
	}

	_messages.clear();
}