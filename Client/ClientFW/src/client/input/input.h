#pragma once

namespace client_fw
{
	class IVec2;
	class InputManager;
	class InputEventManager;

	enum class EAdditionalKey
	{
		kNone = 0x00,
		kShift = 0x10,
		kCtrl = 0x11,
		kAlt = 0x12,
	};

	struct EventKeyInfo
	{
		UINT key;
		EAdditionalKey additional_key = EAdditionalKey::kNone;
	}; 

	struct AxisEventKeyInfo
	{
		UINT key;
		float scale;
	};

	class Input final
	{
	public:
		static bool IsKeyHoldDown(UINT key);
		static bool IsKeyPressed(UINT key);
		static bool IsKeyReleased(UINT key);
		static bool IsNotKeyHoldDown(UINT key);
		static const IVec2& GetMousePosition();
		static const IVec2 GetRelativeMousePosition();

		static void ConsumeKey(UINT key);
		static bool IsConsumedKey(UINT key);

		static void SetHideCursor(bool hide);
		static bool IsHideCursor();

		static void SetClipCursor(bool clip);
		static bool IsClipCursor();

	private:
		friend InputManager;
		static InputManager* s_input_manager;
	};
}


//Key
#define INPUT_MOUSE_LBUTTON		0x01
#define INPUT_MOUSE_RBUTTON		0x02
#define INPUT_MOUSE_MBUTTON		0x04
#define INPUT_MOUSE_XBUTTON1	0x05
#define INPUT_MOUSE_XBUTTON2	0x06
#define INPUT_MOUSE_XMOVE		0xEA
#define INPUT_MOUSE_YMOVE		0xEB

#define INPUT_KEY_BACKSPACE		0x08
#define INPUT_KEY_TAB			0x09
#define INPUT_KEY_ENTER			0x0D
#define INPUT_KEY_SHIFT			0x10
#define INPUT_KEY_CONTROL		0x11
#define INPUT_KEY_ALT			0x12
#define INPUT_KEY_PAUSE			0x13
#define INPUT_KEY_CAPSLOCK		0x14
#define INPUT_KEY_ESCAPE		0x1B

#define INPUT_KEY_SPACE			0x20
#define INPUT_KEY_PAGEUP		0x21
#define INPUT_KEY_PAGEDOWN		0x22
#define INPUT_KEY_END			0x23
#define INPUT_KEY_HOME			0x24
#define INPUT_KEY_LARROW		0x25
#define INPUT_KEY_UARROW		0x26
#define INPUT_KEY_RARROW		0x27
#define INPUT_KEY_DARROW		0x28
#define INPUT_KEY_SELECT		0x29
#define INPUT_KEY_PRINT			0x2A
#define INPUT_KEY_EXECUTE		0x2B
#define INPUT_KEY_SNAPSHOT		0x2C
#define INPUT_KEY_INSERT		0x2D
#define INPUT_KEY_DELETE		0x2E
#define INPUT_KEY_HELP			0x2F

#define INPUT_KEY_0				0x30
#define INPUT_KEY_1				0x31
#define INPUT_KEY_2				0x32
#define INPUT_KEY_3				0x33
#define INPUT_KEY_4				0x34
#define INPUT_KEY_5				0x35
#define INPUT_KEY_6				0x36
#define INPUT_KEY_7				0x37
#define INPUT_KEY_8				0x38
#define INPUT_KEY_9				0x39

#define INPUT_KEY_A				0x41
#define INPUT_KEY_B				0x42
#define INPUT_KEY_C				0x43
#define INPUT_KEY_D				0x44
#define INPUT_KEY_E				0x45
#define INPUT_KEY_F				0x46
#define INPUT_KEY_G				0x47
#define INPUT_KEY_H				0x48
#define INPUT_KEY_I				0x49
#define INPUT_KEY_J				0x4A
#define INPUT_KEY_K				0x4B
#define INPUT_KEY_L				0x4C
#define INPUT_KEY_M				0x4D
#define INPUT_KEY_N				0x4E
#define INPUT_KEY_O				0x4F
#define INPUT_KEY_P				0x50
#define INPUT_KEY_Q				0x51
#define INPUT_KEY_R				0x52
#define INPUT_KEY_S				0x53
#define INPUT_KEY_T				0x54
#define INPUT_KEY_U				0x55
#define INPUT_KEY_V				0x56
#define INPUT_KEY_W				0x57
#define INPUT_KEY_X				0x58
#define INPUT_KEY_Y				0x59
#define INPUT_KEY_Z				0x5A

#define INPUT_KEY_NUMPAD0		0x60
#define INPUT_KEY_NUMPAD1		0x61
#define INPUT_KEY_NUMPAD2		0x62
#define INPUT_KEY_NUMPAD3		0x63
#define INPUT_KEY_NUMPAD4		0x64
#define INPUT_KEY_NUMPAD5		0x65
#define INPUT_KEY_NUMPAD6		0x66
#define INPUT_KEY_NUMPAD7		0x67
#define INPUT_KEY_NUMPAD8		0x68
#define INPUT_KEY_NUMPAD9		0x69
#define INPUT_KEY_MULTIPLY		0x6A
#define INPUT_KEY_ADD			0x6B
#define INPUT_KEY_SEPARATOR		0x6C
#define INPUT_KEY_SUBTRACT		0x6D
#define INPUT_KEY_DECIMAL		0x6E
#define INPUT_KEY_DIVIDE		0x6F
#define INPUT_KEY_F1			0x70
#define INPUT_KEY_F2			0x71
#define INPUT_KEY_F3			0x72
#define INPUT_KEY_F4			0x73
#define INPUT_KEY_F5			0x74
#define INPUT_KEY_F6			0x75
#define INPUT_KEY_F7			0x76
#define INPUT_KEY_F8			0x77
#define INPUT_KEY_F9			0x78
#define INPUT_KEY_F10			0x79
#define INPUT_KEY_F11			0x7A
#define INPUT_KEY_F12			0x7B

#define INPUT_KEY_BACKTICK		0xC0