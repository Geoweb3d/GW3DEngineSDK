//////////////////////////////////////////////////////////////////////////////
//
// Geoweb3d SDK
// Copyright (c) Geoweb3d, 2008-2023, all rights reserved.
//
// This code can be used only under the rights granted to you by the specific
// Geoweb3d SDK license under which the SDK provided.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../core/GW3DInterFace.h"
#include <Geoweb3dCore/Geoweb3dTypes.h>

/* Primary namespace */
namespace Geoweb3d
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // namespace: Key
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace Key
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Definition of key codes for keyboard events. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum Code
        {
            /// <summary>	A key. </summary>
            A = 'a',
            /// <summary>	B key. </summary>
            B = 'b',
            /// <summary>	C key. </summary>
            C = 'c',
            /// <summary>	D key. </summary>
            D = 'd',
            /// <summary>	E key. </summary>
            E = 'e',
            /// <summary>	F key. </summary>
            F = 'f',
            /// <summary>	G key. </summary>
            G = 'g',
            /// <summary>	H key. </summary>
            H = 'h',
            /// <summary>	I key. </summary>
            I = 'i',
            /// <summary>	J key. </summary>
            J = 'j',
            /// <summary>	K key. </summary>
            K = 'k',
            /// <summary>	L key. </summary>
            L = 'l',
            /// <summary>	M key. </summary>
            M = 'm',
            /// <summary>	N key. </summary>
            N = 'n',
            /// <summary>	O key. </summary>
            O = 'o',
            /// <summary>	P key. </summary>
            P = 'p',
            /// <summary>	Q key. </summary>
            Q = 'q',
            /// <summary>	R key. </summary>
            R = 'r',
            /// <summary>	S key. </summary>
            S = 's',
            /// <summary>	T key. </summary>
            T = 't',
            /// <summary>	U key. </summary>
            U = 'u',
            /// <summary>	V key. </summary>
            V = 'v',
            /// <summary>	W key. </summary>
            W = 'w',
            /// <summary>	X key. </summary>
            X = 'x',
            /// <summary>	Y key. </summary>
            Y = 'y',
            /// <summary>	Z key. </summary>
            Z = 'z',
            /// <summary>	0 key. </summary>
            Num0 = '0',
            /// <summary>	1 key. </summary>
            Num1 = '1',
            /// <summary>	2 key. </summary>
            Num2 = '2',
            /// <summary>	3 key. </summary>
            Num3 = '3',
            /// <summary>	4 key. </summary>
            Num4 = '4',
            /// <summary>	5 key. </summary>
            Num5 = '5',
            /// <summary>	6 key. </summary>
            Num6 = '6',
            /// <summary>	7 key. </summary>
            Num7 = '7',
            /// <summary>	8 key. </summary>
            Num8 = '8',
            /// <summary>	9 key. </summary>
            Num9 = '9',
            /// <summary>	Escape key. </summary>
            Escape = 256,
            /// <summary>	left Control key. </summary>
            LControl,
            /// <summary>	left Shift key. </summary>
            LShift,
            /// <summary>	left Alt key. </summary>
            LAlt,
            /// <summary>	left OS specific key : windows (Windows and Linux), apple (MacOS X), .... </summary>
            LSystem,
            /// <summary>	right Control key. </summary>
            RControl,
            /// <summary>	right Shift key. </summary>
            RShift,
            /// <summary>	right Alt key. </summary>
            RAlt,
            /// <summary>	right OS specific key : windows (Windows and Linux), apple (MacOS X), .... </summary>
            RSystem,
            /// <summary>	Menu key. </summary>
            Menu,
            /// <summary>	[ key. </summary>
            LBracket,
            /// <summary>	] key. </summary>
            RBracket,
            /// <summary>	; key. </summary>
            SemiColon,
            /// <summary>	, key. </summary>
            Comma,
            /// <summary>	. key. </summary>
            Period,
            /// <summary>	' key. </summary>
            Quote,
            /// <summary>	/ key. </summary>
            Slash,
            /// <summary>	\ key. </summary>
            BackSlash,
            /// <summary>	~ key. </summary>
            Tilde,
            /// <summary>	= key. </summary>
            Equal,
            /// <summary>	- key. </summary>
            Dash,
            /// <summary>	Space key. </summary>
            Space,
            /// <summary>	Return key. </summary>
            Return,
            /// <summary>	Backspace key. </summary>
            Back,
            /// <summary>	Tabulation key. </summary>
            Tab,
            /// <summary>	Page up key. </summary>
            PageUp,
            /// <summary>	Page down key. </summary>
            PageDown,
            /// <summary>	End key. </summary>
            End,
            /// <summary>	Home key. </summary>
            Home,
            /// <summary>	Insert key. </summary>
            Insert,
            /// <summary>	Delete key. </summary>
            Delete,
            /// <summary>	+ key. </summary>
            Add,
            /// <summary>	- key. </summary>
            Subtract,
            /// <summary>	* key. </summary>
            Multiply,
            /// <summary>	/ key. </summary>
            Divide,
            /// <summary>	Left arrow. </summary>
            Left,
            /// <summary>	Right arrow. </summary>
            Right,
            /// <summary>	Up arrow. </summary>
            Up,
            /// <summary>	Down arrow. </summary>
            Down,
            /// <summary>	numpad 0 key. </summary>
            Numpad0,
            /// <summary>	numpad 1 key. </summary>
            Numpad1,
            /// <summary>	numpad 2 key. </summary>
            Numpad2,
            /// <summary>	numpad 3 key. </summary>
            Numpad3,
            /// <summary>	numpad 4 key. </summary>
            Numpad4,
            /// <summary>	numpad 5 key. </summary>
            Numpad5,
            /// <summary>	numpad 6 key. </summary>
            Numpad6,
            /// <summary>	numpad 7 key. </summary>
            Numpad7,
            /// <summary>	numpad 8 key. </summary>
            Numpad8,
            /// <summary>	numpad 9 key. </summary>
            Numpad9,
            /// <summary>	F1 key. </summary>
            F1,
            /// <summary>	F2 key. </summary>
            F2,
            /// <summary>	F3 key. </summary>
            F3,
            /// <summary>	F4 key. </summary>
            F4,
            /// <summary>	F5 key. </summary>
            F5,
            /// <summary>	F6 key. </summary>
            F6,
            /// <summary>	F7 key. </summary>
            F7,
            /// <summary>	F8 key. </summary>
            F8,
            /// <summary>	F9 key. </summary>
            F9,
            /// <summary>	F10 key. </summary>
            F10,
            /// <summary>	F11 key. </summary>
            F11,
            /// <summary>	F12 key. </summary>
            F12,
            /// <summary>	F13 key. </summary>
            F13,
            /// <summary>	F14 key. </summary>
            F14,
            /// <summary>	F15 key. </summary>
            F15,
            /// <summary>	Pause key. </summary>
            Pause,
            /// <summary>	Do Not Use. </summary>
            Code_Last = 0xFFFF
        };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // namespace: Mouse
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace Mouse
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Definition of button codes for mouse events. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum Button
        {
            /// <summary>	The left mouse button. </summary>
            Left,
            /// <summary>	The right mouse button. </summary>
            Right,
            /// <summary>	The middle (wheel) mouse button. </summary>
            Middle,
            /// <summary>	The first extra mouse button. </summary>
            XButton1,
            /// <summary>	The second extra mouse button. </summary>
            XButton2,
            /// <summary>	Keep last -- the total number of mouse buttons. </summary>
            ButtonCount
        };
    }

    namespace GW3DOpenGL
    {

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum GLCode
        {
            MakeContextCurrent,
            GLCode_Code_Last = 0xFFFF
        };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Window event. </summary>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    class GW3D_DLL WindowEvent
    {
    public:

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Key event. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct KeyEvent
        {
            /// <summary>	Code of the key that has been pressed. </summary>
            Key::Code code;
            /// <summary>	Alt key pressed? </summary>
            bool      alt;
            /// <summary>	Control key pressed?</summary>
            bool      control;
            /// <summary>	 Shift key pressed?</summary>
            bool      shift;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL GW3DOpenGLTypeEvent
        {
            GW3DOpenGL::GLCode code;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Text event parameters (TextEntered) </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL TextEvent
        {
            /// <summary>	UTF-32 unicode value of the character. </summary>
            unsigned int unicode;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Mouse move event parameters (MouseMoved) </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL MouseMoveEvent
        {
            /// <summary>	X position of the mouse pointer, relative to the left of the owner window. </summary>
            int x;
            /// <summary>	Y position of the mouse pointer, relative to the top of the owner window. </summary>
            int y;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Mouse buttons events parameters. </summary>
        ///
        /// <remarks>	MouseButtonPressed, MouseButtonReleased. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL MouseButtonEvent
        {
            /// <summary>	Code of the button that has been pressed. </summary>
            Mouse::Button button;
            /// <summary>	X position of the mouse pointer, relative to the left of the owner window. </summary>
            int           x;
            /// <summary>	Y position of the mouse pointer, relative to the top of the owner window. </summary>
            int           y;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Mouse wheel events parameters</summary>
        ///
        /// <remarks>	MouseWheelMoved. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL MouseWheelEvent
        {
            /// <summary>	Number of ticks the wheel has moved (positive is up, negative is down). </summary>
            int delta;
            /// <summary>	X position of the mouse pointer, relative to the left of the owner window. </summary>
            int x;
            /// <summary>	Y position of the mouse pointer, relative to the top of the owner window. </summary>
            int y;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Size events parameters. </summary>
        ///
        /// <remarks>	Resize. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        struct GW3D_DLL SizeEvent
        {
            /// <summary>	New width, in pixels. </summary>
            unsigned int width;

            /// <summary>	New height, in pixels. </summary>
            unsigned int height;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Enumeration of the different types of events. </summary>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        enum EventType
        {
            /// <summary>	window requested to be closed. </summary>
            Closed,
            /// <summary>	window was resized. </summary>
            Resized,
            /// <summary>	window lost the focus. </summary>
            LostFocus,
            /// <summary>	window gained the focus. </summary>
            GainedFocus,
            /// <summary>	character was entered. </summary>
            TextEntered,
            /// <summary>	key was pressed. </summary>
            KeyPressed,
            /// <summary>	key was released. </summary>
            KeyReleased,
            /// <summary>	mouse wheel was scrolled. </summary>
            MouseWheelMoved,
            /// <summary>	mouse button was pressed. </summary>
            MouseButtonPressed,
            /// <summary>	mouse button was released. </summary>
            MouseButtonReleased,
            /// <summary>	mouse button double clicked. </summary>
            MouseButtonDBLClick,
            /// <summary>	mouse cursor moved. </summary>
            MouseMoved,
            /// <summary>	mouse cursor entered the area of the window. </summary>
            MouseEntered,
            /// <summary>	mouse cursor left the area of the window. </summary>
            MouseLeft,
            /// <summary>	if an external opengl context, like SDL/GLUT, SDL_GL_MakeCurrent current must happen if this is called</summary>
            GW3DOpenGLType,
            /// <summary>	Do Not Use. </summary>
            Type_Last = 0xFFFF
        };

        /// <summary>	Type of the event. </summary>
        EventType Type;

        union
        {
            /// <summary>	Key event parameters. </summary>
            KeyEvent         Key;
            /// <summary>	Text event parameters. </summary>
            TextEvent        Text;
            /// <summary>	Mouse move event parameters. </summary>
            MouseMoveEvent   MouseMove;
            /// <summary>	Mouse button event parameters. </summary>
            MouseButtonEvent MouseButton;
            /// <summary>	Mouse wheel event parameters. </summary>
            MouseWheelEvent  MouseWheel;
            /// <summary>	Size event parameters. </summary>
            SizeEvent        Size;
            GW3DOpenGLTypeEvent	 OpenGLEvent;
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// <summary>	Window callback. </summary>
    ///
    /// <remarks>	The window callback is implemented by the SDK client and passed into the SDK at
    /// 			window creation. </remarks>
    ///
    /// <see cref="IGW3DWindowCollection::create_3DWindow"/>
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct GW3D_DLL IGW3DWindowCallback
    {
        virtual ~IGW3DWindowCallback() {}

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	GPU Context created</summary>
        ///
        /// <remarks>	The SDK calls the OnCreate callback once the rendering context is created.
        ///				This is a place clients can initialize their gpu specific items as well.
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnCreate() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Called just before drawing starts. </summary>
        ///
        /// <remarks>	If a client draws or uses the frame buffer at this point, it will get overwritten
        ///             when the sdk starts to draw. </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnDrawBegin() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Called just after all active cameras have been drawn. </summary>
        ///
        /// <remarks>	Note, for same frame 2D drawing per camera, its suggested to use OnDraw2D </remarks>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnDrawEnd() = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Draw 2D Callback. </summary>
        ///
        /// <remarks>	The SDK calls the 2D drawing mode callback during the draw call.  Try to be smart
        /// 			about your OpenGL states.  At the point of calling OnDraw2D, the SDK finished
        /// 			drawing everything, so you can stream out things like what is in the field of view
        /// 			so you know where to place icons etc.
        ///
        /// 			Note, if you have multiple cameras on a window, 'camera' will be the camera that
        ///				has just finished sending the OpenGL commands to the GPU. </remarks>
        ///
        /// <param name="camera">	The camera. </param>
        ///
        /// <see cref="IGW3DGeoweb3dSDK::draw"/>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnDraw2D(const IGW3DCameraWPtr& camera) = 0;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary> TODO </summary>
        ///
        /// <param name="camera"> TODO </param>
        /// <param name="drawcontext"> TODO </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void OnDraw2D(const IGW3DCameraWPtr& camera, IGW3D2DDrawContext* drawcontext)
        {
            //not yet required to implement, other OnDraw2D may become obsolete
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	Process low-level event </summary>
        ///
        /// <remarks>	Return 0 if you handle the message and don't want it to be sent to another handler
        /// 			downstream (the event will not be passed on to other windows).  This includes
        /// 			ProcessEvent( const WindowEvent win_event ).  This is the raw message from the
        /// 			Operating System. </remarks>
        ///
        /// <param name="hWndParent">	The window parent. </param>
        /// <param name="wParam">	 	The wParam field of the message. </param>
        /// <param name="lParam">	 	The lParam field of the message. </param>
        ///
        /// <returns>	0 to stop event from being propagated. </returns>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual int ProcessLowLevelEvent(OSWinHandle hWndParent, int64_t, int64_t wParam, int64_t lParam)
        {
            return 1;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>	A high-level option for processing window events </summary>
        ///
        /// <remarks>	You can use this instead of ProcessLowLevelEvent if it supports all the events
        /// 			you need.  Note, if we created the 3D window off your window, you can still use
        /// 			the events off your own GUI framework. </remarks>
        ///
        /// <param name="win_event">	The window event. </param>
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        virtual void ProcessEvent(const WindowEvent& win_event)
        {
        }

    };
}