#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xcb/xcb.h>

int main()
{
	xcb_connection_t* pConnection;
	xcb_screen_t* pScreen;
	xcb_window_t window;
	xcb_gcontext_t gcForeground;
	xcb_gcontext_t gcBackground;
	xcb_generic_event_t* pEvent;
	uint32_t mask = 0;
	uint32_t values[2] = { 0 };
	bool bIsExit = false;

	const char* szWndCaption = "Hello, xyEngine!";
	const char* szWndIconCaption = "Hello, xyEngine! (iconified)";
	int nClientWidth = 800;
	int nClientHeight = 600;

	/* establish connection to X server */
	pConnection = xcb_connect(0, 0);

	/* get the first screen */
	pScreen = xcb_setup_roots_iterator(xcb_get_setup(pConnection)).data;

	/* get the root window */
	window = pScreen->root;

	/* create black (foreground) graphic context */
	gcForeground = xcb_generate_id(pConnection);
	mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = pScreen->black_pixel;
	values[1] = 0;
	xcb_create_gc(pConnection, gcForeground, window, mask, values);

	/* create white (background) graphic context */
	gcBackground = xcb_generate_id(pConnection);
	mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = pScreen->white_pixel;
	values[1] = 0;
	xcb_create_gc(pConnection, gcBackground, window, mask, values);

	/* create window */
	window = xcb_generate_id(pConnection);
	mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	values[0] = pScreen->white_pixel;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
	xcb_create_window(pConnection,												 /* connection */
										 XCB_COPY_FROM_PARENT,						 /* depth */
										 window,														 /* window ID */
										 pScreen->root,											 /* parent window */
										 0, 0,																 /* x, y */
										 nClientWidth, nClientHeight,					 /* width, height */
										 10,																	 /* boarder width */
										 XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class */
										 pScreen->root_visual,								 /* visual */
										 mask, values);												 /* masks */

	/* set the title of the window */
	xcb_change_property(pConnection, XCB_PROP_MODE_REPLACE, window,
		XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
		strlen(szWndCaption), szWndCaption);

	/* set the title of the window icon */
	xcb_change_property(pConnection, XCB_PROP_MODE_REPLACE, window,
		XCB_ATOM_WM_ICON_NAME, XCB_ATOM_STRING, 8,
		strlen(szWndIconCaption), szWndIconCaption);

	/* map the window on the screen */
	xcb_map_window(pConnection, window);

	xcb_flush(pConnection);

	while ((pEvent = xcb_wait_for_event(pConnection)) && !bIsExit)
	{
		switch (pEvent->response_type & ~0x80)
		{
		case XCB_EXPOSE:
			break;
		case XCB_KEY_PRESS:
			bIsExit = true;
			break;
		default:
			break;
		}

		free(pEvent);
	}

	return 0;
}
