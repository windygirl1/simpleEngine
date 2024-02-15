#pragma once

#include <functional>
#include <array>

namespace SimpleEngine {
	enum class EventType {
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventCount
	};

	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType getType() const = 0;
	};

	class EventDispatcher {
	public:
		template<typename EventType>
		void addEventListener(std::function<void(EventType&)> callback) {
			auto baseCallback = [func = std::move(callback)](BaseEvent& e)
				{
					if (e.getType() == EventType::type) {
						func(static_cast<EventType&>(e));
					}
				};
			eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& e) {
			auto& callback = eventCallbacks[static_cast<size_t>(e.getType())];
			if (callback) {
				callback(e);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventCount)> eventCallbacks;
	};

	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const double newX, const double newY) : x(newX), y(newY) {

		}

		virtual EventType getType() const override {
			return type;
		}

		double x;
		double y;

		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent {
		EventWindowResize(const unsigned int newWidth, const unsigned int newHeight) : width(newWidth), height(newHeight) {

		}

		virtual EventType getType() const override {
			return type;
		}

		unsigned int width;
		unsigned int height;

		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent {
		virtual EventType getType() const override {
			return type;
		}

		static const EventType type = EventType::WindowClose;
	};
}