#pragma once
#include "Event.h"
#include "Framework/Network/NetworkMessage.h"

namespace Framework
{
    class NetworkMessageReceivedEvent : public Event
    {
    public:
        NetworkMessageReceivedEvent(const Ref<NetworkMessage>& aMessage)
            : myNetworkMessage(aMessage) {}

        inline const Ref<NetworkMessage>& GetNetworkMessage() const { return myNetworkMessage; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "NetworkMessage Type: " << myNetworkMessage->GetType();
            return ss.str();
        }

        EVENT_CLASS_TYPE(NetworkMessageReceived)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        Ref<NetworkMessage> myNetworkMessage;
    };
}