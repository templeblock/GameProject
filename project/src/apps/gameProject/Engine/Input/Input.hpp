#ifndef INPUT_HPP
#define INPUT_HPP

/// Internal Includes
#include "../Core/Console.hpp"

/// External Includes
#include <Config\Config.hpp>
#include <RenderEngine/IWindow.hpp>

/// Std Includes
#include <map>
#include <sstream>

namespace Engine {
	namespace Input {

		struct InputMapping {
			int keyboard;
			int mouse;
			int gamepad;
			int moveDir;
		};

		enum class InputStateType {
			E_STATE_NONE,
			E_STATE_PRESSED,
			E_STATE_RELEASED,
			E_STATE_HOLDING,
		};

		struct InputState {
			InputStateType state;
			float value;
		};

		struct InputEvent {
			int code;
			bool mouse;

			inline friend bool operator< (const InputEvent& lhs, const InputEvent& rhs) {
				int l = lhs.code;
				int r = rhs.code;
				if (lhs.mouse)
					l = -(l + 1);
				if (rhs.mouse)
					r = -(r + 1);
				return (l < r);
			};

			inline friend bool operator> (const InputEvent& lhs, const InputEvent& rhs) { return rhs < lhs; }
			inline friend bool operator<=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs > rhs); }
			inline friend bool operator>=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs < rhs); }

			inline friend bool operator== (const InputEvent& lhs, const InputEvent& rhs) { return (lhs.code == rhs.code) && (lhs.mouse == lhs.mouse); };
			inline friend bool operator!=(const InputEvent& lhs, const InputEvent& rhs) { return !(lhs == rhs); }
		};

		struct KeyBind {
			int code;
			int mod;
			bool mouse;

			static inline KeyBind create(std::string data) {
				std::stringstream ss(data);
				int code, mod;
				bool mouse;
				std::string buff;
				std::getline(ss, buff, ':');
				code = std::stoi(buff);

				std::getline(ss, buff, ':');
				mouse = std::stoi(buff) == 1 ? true : false;

				std::getline(ss, buff, ':');
				mod = std::stoi(buff);
				KeyBind kb { code, mod, mouse };

				return kb;
			}

			inline std::string toString() {
				std::string res = std::to_string(code) + ":" + std::to_string(mouse) + ":" + std::to_string(mod);

				return res;
			}
		};

		enum class InputDevice {
			E_DEVICE_MOUSE_KEYBOARD,
			E_DEVICE_GAMEPAD,
		};

		struct Axis {
			float x;
			float y;
		};

		class Input {
		public:
			Input();

			static Input* GetInput();
			static void Release();

			void attachConsole(Core::Console* con);
			void clearCallbacks();
			void setupCallbacks(IWindow* wnd); //Must call this when window is created to make callback functions work!

			bool isKeyBindPressed(const KeyBind &keyBind, bool includeMods = true);
			bool releasedThisFrame(const KeyBind &keyBind, bool includeMods = true);
			bool wasPressedThisFrame(const KeyBind &keyBind, bool includeMods = true);

			InputState checkInputMapping(const InputMapping &mapping);

			void getCursorDelta(float &x, float &y);

			bool consoleKeyWasPressed();
			void toggleConsole();
			bool consoleIsActive();

			InputDevice getLastInputDevice() const;

			void reset();

			void getWindowSize(int &w, int &h);

			void getMousePos(int &x, int &y);

			void getState(int &mx, int &my, int &mb, int &sc);

			void print();

			void getMovingVector(float &x, float &y);

			bool sizeChange;
			bool characterRecieved;
			unsigned int characterThisFrame;
			bool returnPressed;
			bool backspacePressed;

			IWindow* window;

			InputEvent lastPressed;
		private:

			InputDevice lastInputDevice;

			Config keyConf;
			bool consoleActive;

			bool blockInput;
			bool focus;

			static Input* singleton;

			void loadkeyBinds();
			void saveKeyBinds();

			std::map<InputEvent, bool> keyMap;
			std::map<InputEvent, bool> releaseMap;
			std::map<InputEvent, bool> pressedMap;
			int modkey;
			
			std::map<int, InputState> keyboardMap;
			std::map<int, InputState> mouseMap;
			std::map<int, InputState> gamepadMap;

			int xPos;
			int yPos;

			int oldY;
			int oldX;

			float xDelta;
			float yDelta;

			float scrollX;
			float scrollY;

			int winW;
			int winH;

			float inputSensitivity;

			Core::Console* console;

			Axis rThumb;
			Axis lThumb;
			Axis triggers;

			static void keyCallback(IWindow* window, int scancode, int action, int mods);
			static void mouseButtonCallback(IWindow* window, int button, int action, int mods);
			static void cursorPosCallback(IWindow* window, int x, int y);
			static void scrollCallback(IWindow* window, int xoffset, int yoffset);
			static void characterCallback(IWindow* window, unsigned int codepoint);
			static void sizeCallback(IWindow* window, int w, int h);

			static void focusCallback(IWindow* window, bool focus);

			static void mouseDeltaCallback(IWindow* window, float dx, float dy);

			static void controllerAxisCallback(IWindow* window, unsigned int axis, float axisValue);
			static void controllerButtonCallback(IWindow* window, unsigned int button, int action);

		};

		enum KEYBINDS_NAME {
			KEYBIND_FORWARD = 0,
			KEYBIND_BACKWARD,
			KEYBIND_LEFT,
			KEYBIND_RIGHT,
			KEYBIND_ENTER,
			KEYBIND_MOUSE_L_CLICK,
			KEYBIND_MOUSE_WHEEL_CLICK,
			KEYBIND_UP_ARROW,
			KEYBIND_DOWN_ARROW,
			KEYBIND_LEFT_ARROW,
			KEYBIND_RIGHT_ARROW,
			KEYBIND_SPACE,
			KEYBINDS_LENGTH,
		};

		static const char* KeyBindsStrings[] = {
			"KeyAction_Forward",
			"KeyAction_Backward",
			"KeyAction_Left",
			"KeyAction_Right",
			"KeyAction_Enter",
			"KeyAction_Mouse_L_Click",
			"KeyAction_Mouse_WHEEL_Click",
			"KeyAction_Arrow_UP",
			"KeyAction_Arrow_DOWN",
			"KeyAction_Arrow_LEFT",
			"KeyAction_Arrow_RIGHT",
			"KeyAction_Space",
		};

		static const char* KeyBindsDefault[] = {
			"17:0:0",
			"31:0:0",
			"30:0:0",
			"32:0:0",
			"28:0:0",
			"0:1:0",
			"2:1:0",
			"72:0:0",
			"80:0:0",
			"75:0:0",
			"77:0:0",
			"57:0:0",
		};

		extern KeyBind KeyBindings[];

	}
}
#endif