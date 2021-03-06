#pragma once
#include "precompile.h"
#include "Framework/Core/Core.h"

namespace Framework
{
    // Events in Framework are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
        NetworkMessageReceive
    };

    enum EventCategory : int
    {
        None = 0,
        EventCategoryApplication =      BIT(0),
        EventCategoryInput =            BIT(1),
        EventCategoryKeyboard =         BIT(2),
        EventCategoryMouse =            BIT(3),
        EventCategoryMouseButton =      BIT(4),
        EventCategoryNetworkMessage =   BIT(5),
    };


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public: 
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

        bool myHandled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : myEvent(event)
        {
        }

        // F will be deduced by the compiler
        template<typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (myEvent.GetEventType() == T::GetStaticType())
            {
                myEvent.myHandled = func(static_cast<T&>(myEvent));
                return true;
            }
            return false;
        }
    private:
        Event& myEvent;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}