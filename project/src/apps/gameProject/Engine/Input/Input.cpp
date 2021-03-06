/// Internal Includes
#include "Input.hpp"
#include "ScanCodes.hpp"
///External Includes

/// Std Includes
#include <iostream>

namespace Engine {
	namespace Input {

		Input* Input::singleton = nullptr;
		KeyBind KeyBindings[KEYBINDS_NAME::KEYBINDS_LENGTH];

		void Input::keyCallback(IWindow* , int scancode, int action, int mods) {
			singleton->lastInputDevice = InputDevice::E_DEVICE_MOUSE_KEYBOARD;
			if (!singleton->consoleActive && action == 2) {
				return;
			}

			//singleton->keyMap[InputEvent{ scancode, false }] = (action == ACTION_BUTTON_DOWN);

			singleton->modkey = mods;
			if ((scancode == 28 || scancode == 284) && action == ACTION_BUTTON_DOWN) {
				if (singleton->consoleActive) {
					singleton->console->execute();
				}
				singleton->returnPressed = true;

			} else if (scancode == 14 && (action == ACTION_BUTTON_DOWN || action == 2)) {
				if (singleton->consoleActive) {
					//gConsole->removeLastChar();
					singleton->console->backSpace();
				}
				singleton->backspacePressed = true;
			}
			const InputEvent inEvent = { scancode, false };
			singleton->lastPressed = inEvent;
			//printf("Scancode %d with modkey %d\n", scancode, mods);


			if (action == ACTION_BUTTON_UP) {
				singleton->releaseMap[inEvent] = true;
				singleton->keyboardMap[scancode] = InputState { InputStateType::E_STATE_RELEASED, 0.0F };
			} else if (action == ACTION_BUTTON_DOWN) {
				singleton->pressedMap[inEvent] = true;
				singleton->keyboardMap[scancode] = InputState { InputStateType::E_STATE_PRESSED, 0.0F };
			}
		}

		void Input::mouseButtonCallback(IWindow* , int button, int action, int ) {
			singleton->lastInputDevice = InputDevice::E_DEVICE_MOUSE_KEYBOARD;
			if (!singleton->consoleActive && action == 2) {
				return;
			}

			const InputEvent inEvent = { button , true };
			singleton->lastPressed = inEvent;

			//printf("Mouse callback\n");

			//singleton->keyMap[InputEvent{ button, true }] = (action == ACTION_BUTTON_DOWN);
			if (action == ACTION_BUTTON_UP) {
				singleton->releaseMap[inEvent] = true;
				singleton->mouseMap[button] = InputState { InputStateType::E_STATE_RELEASED, 0.0F };
			} else if (action == ACTION_BUTTON_DOWN) {
				singleton->pressedMap[inEvent] = true;
				singleton->mouseMap[button] = InputState { InputStateType::E_STATE_PRESSED, 0.0F };
			}
			//printf("MouseButton %d with modkey %d\n", button, mods);
		}

		void Input::cursorPosCallback(IWindow * , int x, int y) {
			if (singleton->blockInput) {
				return;
			}
			//singleton->xDelta = float(x) - float(singleton->oldX);
			//singleton->yDelta = float(y) - float(singleton->oldY);

			singleton->xPos = int(x);
			singleton->yPos = int(y);

			singleton->blockInput = true;
			//printf("Pos (%d,%d)\n", int(x), int(y));
			//printf("Delta (%f,%f)\n", singleton->xDelta, singleton->yDelta);
		}

		void Input::scrollCallback(IWindow * , int xoffset, int yoffset) {
			singleton->scrollX = float(xoffset);
			singleton->scrollY = float(yoffset);

			//printf("Scroll (%f,%f)\n", float(xoffset), float(yoffset));
		}

		void Input::characterCallback(IWindow * , unsigned int codepoint) {
			if (singleton->consoleActive) {
				//printf("%c", codepoint);
				singleton->console->putChar(static_cast<char>(codepoint));
				//singleton->console->keyPress(codepoint);
			}

			singleton->characterRecieved = true;
			singleton->characterThisFrame = codepoint;

		}

		void Input::sizeCallback(IWindow * , int w, int h) {
			singleton->winW = w;
			singleton->winH = h;
			singleton->sizeChange = true;
			//printf("%d,%d\n", w, h);
			//glViewport(0, 0, w, h);
		}

		void Input::focusCallback(IWindow * , bool focus) {
			singleton->focus = focus;
			//printf("focus %d\n", focus);
			if (focus == false) {
				//window->lockCursor(false);
				singleton->keyMap.clear();
				singleton->gamepadMap.clear();
				singleton->mouseMap.clear();
				singleton->keyboardMap.clear();
				//printf("Clearing key mappings\n");
			}
		}

		void Input::mouseDeltaCallback(IWindow * , float dx, float dy) {
			singleton->lastInputDevice = InputDevice::E_DEVICE_MOUSE_KEYBOARD;
			singleton->xDelta += dx * singleton->inputSensitivity;// * 4.4f;
			singleton->yDelta += dy * singleton->inputSensitivity;// * 4.4f;

			//printf("Delta (%f,%f)\n", dx, dy);
		}

		void Input::controllerAxisCallback(IWindow* , unsigned int axis, float axisValue) {
			singleton->lastInputDevice = InputDevice::E_DEVICE_GAMEPAD;

			if (axis == GAMEPAD_AXIS_RX) {
				singleton->rThumb.x = axisValue;
			} else if (axis == GAMEPAD_AXIS_RY) {
				singleton->rThumb.y = axisValue;
			} else if (axis == GAMEPAD_AXIS_LX) {
				singleton->lThumb.x = axisValue;
			} else if (axis == GAMEPAD_AXIS_LY) {
				singleton->lThumb.y = axisValue;
			} else if (axis == GAMEPAD_AXIS_RT) {
				singleton->triggers.x = axisValue;
			} else if (axis == GAMEPAD_AXIS_LT) {
				singleton->triggers.y = axisValue;
			}

		}

		void Input::controllerButtonCallback(IWindow* , unsigned int button, int action) {
			singleton->lastInputDevice = InputDevice::E_DEVICE_GAMEPAD;

			int id = 0;

			switch (button) {
			case GAMEPAD_BTN_A:
				id = GamePad::BTN_A;
				break;
			case GAMEPAD_BTN_B:
				id = GamePad::BTN_B;
				break;
			case GAMEPAD_BTN_X:
				id = GamePad::BTN_X;
				break;
			case GAMEPAD_BTN_Y:
				id = GamePad::BTN_Y;
				break;
			case GAMEPAD_BTN_BACK:
				id = GamePad::BTN_BACK;
				break;
			case GAMEPAD_BTN_START:
				id = GamePad::BTN_START;
				break;
			case GAMEPAD_BTN_DP_DOWN:
				id = GamePad::BTN_D_DOWN;
				break;
			case GAMEPAD_BTN_DP_LEFT:
				id = GamePad::BTN_D_LEFT;
				break;
			case GAMEPAD_BTN_DP_RIGHT:
				id = GamePad::BTN_D_RIGHT;
				break;
			case GAMEPAD_BTN_DP_UP:
				id = GamePad::BTN_D_UP;
				break;
			case GAMEPAD_BTN_LT:
				id = GamePad::BTN_BUMP_LEFT;
				break;
			case GAMEPAD_BTN_RT:
				id = GamePad::BTN_BUMP_RIGHT;
				break;
			case GAMEPAD_BTN_L_THUMB:
				id = GamePad::BTN_THUMB_LEFT;
				break;
			case GAMEPAD_BTN_R_THUMB:
				id = GamePad::BTN_THUMB_RIGHT;
				break;
			default:
				id = 0;
				break;
			}

			if (action == ACTION_BUTTON_UP) {
				singleton->gamepadMap[id] = InputState { InputStateType::E_STATE_RELEASED, 0.0F };
			} else if (action == ACTION_BUTTON_DOWN) {
				singleton->gamepadMap[id] = InputState { InputStateType::E_STATE_PRESSED, 0.0F };
			}
		}

		Input* Input::GetInput() {
			if (singleton == nullptr)
				singleton = new Input();
			return singleton;
		}

		void Input::Release() {
			if (singleton != nullptr) {
				if (singleton->window) {
					singleton->clearCallbacks();
				}
				singleton->saveKeyBinds();
				singleton->keyConf.write("Config/keybinds.ini");
				delete singleton;
			}
			singleton = nullptr;
		}

		void Input::attachConsole(Core::Console* con) {
			console = con;
		}

		void Input::clearCallbacks() {
			if (window) {
				window->setWindowKeyboardCallback(nullptr);
				window->setWindowMouseButtonCallback(nullptr);
				window->setWindowMouseMoveCallback(nullptr);
				window->setWindowScrollCallback(nullptr);
				window->setWindowCharacterCallback(nullptr);
				window->setWindowResizeCallback(nullptr);

				window->setWindowFocusCallback(nullptr);
			}
		}

		void Input::setupCallbacks(IWindow * wnd) {
			clearCallbacks();
			window = wnd;

			window->setWindowKeyboardCallback(keyCallback);
			window->setWindowMouseButtonCallback(mouseButtonCallback);
			window->setWindowMouseMoveCallback(cursorPosCallback);
			window->setWindowScrollCallback(scrollCallback);
			window->setWindowCharacterCallback(characterCallback);
			window->setWindowResizeCallback(sizeCallback);

			window->setWindowFocusCallback(focusCallback);

			window->setWindowMouseDeltaCallback(mouseDeltaCallback);

			window->setWindowControllerAxisCallback(controllerAxisCallback);
			window->setWindowControllerButtonCallback(controllerButtonCallback);

		}

		bool Input::isKeyBindPressed(const KeyBind & keyBind, bool includeMods) {
			auto it = keyMap.find(InputEvent { keyBind.code, keyBind.mouse });
			bool pressed = false;
			if (it != keyMap.end()) {
				pressed = it->second;
				if (includeMods) {
					pressed &= (keyBind.mod == modkey);
				}
			}
			return pressed & focus;
		}

		bool Input::releasedThisFrame(const KeyBind & keyBind, bool includeMods) {
			auto it = releaseMap.find(InputEvent { keyBind.code, keyBind.mouse });
			bool released = false;

			if (it != releaseMap.end()) {
				released = it->second;
				if (includeMods) {
					released &= (keyBind.mod == modkey);
				}
			}
			return released & focus;
		}

		bool Input::wasPressedThisFrame(const KeyBind & keyBind, bool includeMods) {
			auto it = pressedMap.find(InputEvent { keyBind.code, keyBind.mouse });
			bool pressed = false;

			if (it != pressedMap.end()) {
				pressed = it->second;
				bool isHeldDown = isKeyBindPressed(keyBind, includeMods);
				if (isHeldDown) {
					pressed = false;
				}
				if (includeMods) {
					pressed &= (keyBind.mod == modkey);
				}
			}
			return pressed & focus;
		}

		InputState Input::checkInputMapping(const InputMapping &mapping) {

			InputState state;

			state.state = InputStateType::E_STATE_NONE;
			state.value = 0.0F;

			if (lastInputDevice == InputDevice::E_DEVICE_GAMEPAD) {

				if (mapping.gamepad != GamePad::BTN_NONE) {
					state = gamepadMap[mapping.gamepad];
				}

			} else {

				if (mapping.keyboard != ScanCodes::SCAN_CODE_NONE) {
					state = keyboardMap[mapping.keyboard];
				}
				if (state.state == InputStateType::E_STATE_NONE || state.state == InputStateType::E_STATE_RELEASED) {
					if (mapping.mouse != Mouse::MOUSE_NONE) {
						state = mouseMap[mapping.mouse];
					}
				}

			}
			return state;
		}

		void Input::getCursorDelta(float & x, float & y) {
			if (lastInputDevice == InputDevice::E_DEVICE_GAMEPAD) {
				x = (rThumb.x) / (255.0F / 10.0F);
				y = (-rThumb.y) / (255.0F / 10.0F);

			} else {
				x = xDelta;
				y = yDelta;
			}
		}

		bool Input::consoleKeyWasPressed() {
			const InputEvent inEvent { 41, false };
			std::map<InputEvent, bool>::iterator it = keyMap.find(inEvent);
			//keyMap[InputEvent{ 41, false }] = false;
			bool pressed = false;
			if (it != keyMap.end()) {
				pressed = it->second;
				it->second = false;
			}
			return pressed;
		}

		void Input::toggleConsole() {
			//printf("Console toggle\n");
			/*if (consoleActive) {
				console->backSpace();
			}*/

			consoleActive = !consoleActive;
		}

		bool Input::consoleIsActive() {
			return consoleActive;
		}

		InputDevice Input::getLastInputDevice() const {
			return lastInputDevice;
		}

		void Input::reset() {
			//printf("Pos (%d,%d)\n", int(xPos), int(yPos));
			//printf("Delta (%f,%f)\n", singleton->xDelta, singleton->yDelta);

			characterRecieved = false;
			characterThisFrame = 0U;
			returnPressed = false;
			backspacePressed = false;

			blockInput = false;
			xDelta = 0.0f;
			yDelta = 0.0;
			scrollX = 0.0;
			scrollY = 0.0;

			sizeChange = false;
			oldX = xPos;
			oldY = yPos;

			std::map<InputEvent, bool>::iterator it = pressedMap.begin();
			std::map<InputEvent, bool>::iterator eit = pressedMap.end();

			for (it; it != eit; it++) {
				keyMap[it->first] = it->second;
			}

			it = releaseMap.begin();
			eit = releaseMap.end();

			for (it; it != eit; it++) {
				if (it->second) {
					keyMap[it->first] = false;
				}
			}

			releaseMap.clear();
			pressedMap.clear();
		}

		void Input::getWindowSize(int & w, int & h) {
			w = winW;
			h = winH;

			if (w < 1280) {
				w = 1280;
			}

			if (h < 720) {
				h = 720;
			}

		}

		void Input::getMousePos(int & x, int & y) {
			window->getCursorPos(x, y);
		}

		void Input::getState(int &mx, int &my, int &mb, int &sc) {
			mx = xPos;
			my = yPos;

			const InputEvent inEvent0 { 0, true };
			const InputEvent inEvent1 { 1, true };
			const InputEvent inEvent2 { 2, true };
			const InputEvent inEvent3 { 3, true };
			const InputEvent inEvent4 { 4, true };

			if (keyMap[inEvent0] == true)
				mb += 1;
			if (keyMap[inEvent1] == true)
				mb += 2;
			if (keyMap[inEvent2] == true)
				mb += 4;
			if (keyMap[inEvent3] == true)
				mb += 8;
			if (keyMap[inEvent4] == true)
				mb += 16;
			sc = (int)scrollY;
		}

		void Input::print() {
			// show content:
			//for ( std::map<InputEvent, bool>::iterator it = keyMap.begin(); it != keyMap.end(); ++it )
			//	std::cout << it->first.code << " " << it->first.mouse << " => " << it->second << '\n';
			if (!releaseMap.empty())
				for (std::map<InputEvent, bool>::iterator it = releaseMap.begin(); it != releaseMap.end(); ++it)
					std::cout << it->first.code << " " << it->first.mouse << " => " << it->second << '\n';
		}

		void Input::getMovingVector(float &x, float &y) {

			if (lastInputDevice == InputDevice::E_DEVICE_GAMEPAD) {
				x = (-lThumb.x / 255.0F);
				y = (lThumb.y / 255.0F);

			} else {

				x = 0;
				y = 0;
				if (isKeyBindPressed(KeyBindings[KEYBIND_FORWARD], false)) {
					y = 1.0F;
				}
				if (isKeyBindPressed(KeyBindings[KEYBIND_BACKWARD], false)) {
					y = -1.0F;
				}
				if (isKeyBindPressed(KeyBindings[KEYBIND_LEFT], false)) {
					x = 1.0F;
				}
				if (isKeyBindPressed(KeyBindings[KEYBIND_RIGHT], false)) {
					x = -1.0F;
				}
				//x = lThumb.x;
				//y = lThumb.y;
			}

		}

		Input::Input()
			: sizeChange(false)
			, characterRecieved(false)
			, characterThisFrame(0U)
			, returnPressed(false)
			, backspacePressed(false)
			, window(nullptr)
			, lastPressed()
			, lastInputDevice()
			, keyConf()
			, consoleActive(false)
			, blockInput(false)
			, focus(false)
			, keyMap()
			, releaseMap()
			, pressedMap()
			, modkey(0)
			, keyboardMap()
			, mouseMap()
			, gamepadMap()
			, xPos(0)
			, yPos(0)
			, oldY(0)
			, oldX(0)
			, xDelta(0.0F)
			, yDelta(0.0F)
			, scrollX(0.0F)
			, scrollY(0.0F)
			, winW(0)
			, winH(0)
			, inputSensitivity(0.0F)
			, console(nullptr)
			, rThumb()
			, lThumb()
			, triggers()

		{
			consoleActive = false;
			window = nullptr;
			keyMap.clear();
			modkey = 0;

			xPos = 0;
			yPos = 0;

			xDelta = 0.0F;
			yDelta = 0.0F;

			scrollX = 0.0F;
			scrollY = 0.0F;

			inputSensitivity = 1.0F;

			keyConf.read("Config/keybinds.ini");
			loadkeyBinds();

			winH = winW = 0;
			sizeChange = false;
			focus = true;

			lastInputDevice = InputDevice::E_DEVICE_MOUSE_KEYBOARD;

		}

		void Input::loadkeyBinds() {
			//std::string bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_FORWARD], "17:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_FORWARD] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_BACKWARD], "31:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_BACKWARD] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_LEFT], "30:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_LEFT] = KeyBind::create(bindingData);
			//
			//bindingData = keyConf.getString("", KeyBindsStrings[KEYBIND_RIGHT], "32:0:0");
			//KeyBindings[KEYBINDS_NAME::KEYBIND_RIGHT] = KeyBind::create(bindingData);

			std::string bindingData = "";
			for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++) {
				bindingData = keyConf.getString("", KeyBindsStrings[i], KeyBindsDefault[i]);
				KeyBindings[i] = KeyBind::create(bindingData);
			}
		}

		void Input::saveKeyBinds() {
			for (size_t i = 0; i < KEYBINDS_NAME::KEYBINDS_LENGTH; i++) {
				keyConf.addString("", KeyBindsStrings[i], KeyBindings[i].toString());
			}
			//keyConf.addString("", KeyBindsStrings[KEYBIND_FORWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_FORWARD].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_BACKWARD], KeyBindings[KEYBINDS_NAME::KEYBIND_BACKWARD].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_LEFT], KeyBindings[KEYBINDS_NAME::KEYBIND_LEFT].toString());
			//keyConf.addString("", KeyBindsStrings[KEYBIND_RIGHT], KeyBindings[KEYBINDS_NAME::KEYBIND_RIGHT].toString());
		}
	}
}