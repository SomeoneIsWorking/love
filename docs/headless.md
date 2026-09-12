# Update-only headless mode

`love --headless path/to/game` runs `love.load` and `love.update` without a
window or display server. Filesystem, data, timer, and
event modules remain available. Window, graphics, audio, and physical-input
modules are disabled even when `love.conf` requests them; `love.draw` is never
called. Games can check `love._headless` when they need a different exit rule.

This mode is for simulation and control-channel tests, not rendering or
screenshots. An application may use the optional [Lucent probe](lucent-probe.md)
to issue input commands and read state while updates run.

The built-in fixture validates module overrides, argument forwarding, three
update iterations, and a zero exit without valid video or audio drivers:

```sh
env -u DISPLAY -u WAYLAND_DISPLAY SDL_VIDEODRIVER=invalid SDL_AUDIODRIVER=invalid \
  build/love --headless testing/headless --fixture-argument
```
