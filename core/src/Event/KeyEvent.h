#pragma once
#include "Event.h"

namespace LearnOpenGL
{
	class KeyEvent :public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode)
		{

		}
		int m_KeyCode;
	};

	class KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCounts)
			: KeyEvent(keyCode), m_RepeatCounts(repeatCounts)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: KeyCode:" << m_KeyCode << ", RepeatCounts" << m_RepeatCounts;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);		
	private:
		int m_RepeatCounts;
	};

	class KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: KeyCode:" << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}