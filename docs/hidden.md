# Hidden-window mode

`love --hidden path/to/game` runs the game with full graphics on the user's own
display session, but the window is created unmapped (`SDL_WINDOW_HIDDEN`), never
raised, restored or focused, and presents without vsync whatever `love.conf` or
`love.window.setVSync` request: an unmapped surface receives no frame callbacks,
so a vsynced present would block. Message boxes print `title: message` to stderr
and return as if dismissed, and the default error handler prints the traceback and
exits with status 1 instead of drawing an error screen. Games can check
`love._hidden`.

This mode is for rendering tests and screenshot capture. It needs no Xvfb,
offscreen video driver or stripped `DISPLAY`/`WAYLAND_DISPLAY`; for runs with no
display at all use [headless mode](headless.md).

The fixture checks the unmapped window, the forced vsync, the logged message box
and a captured frame:

```sh
build/love --hidden testing/hidden /path/to/out.png
```
