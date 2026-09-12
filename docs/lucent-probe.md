# Optional Lucent probe

The `LOVE_PROBE` build option adds a loopback-only control channel for interactive
development and automated observation. The generic LÖVE module owns bounded HTTP
transport and queues; an application decides which commands and state it exposes.

The engine's [update-only headless mode](headless.md) can run a probed game without
a display server, graphics, or audio.

The probe consumes [SomeoneIsWorking/lucent](https://github.com/SomeoneIsWorking/lucent)
at revision `4c72484ff1e0176fe1dedf2803afae3831703afe`. Clone that repository separately,
check out the recorded revision, and configure this fork with its path:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DLOVE_PROBE=ON \
  -DLOVE_LUCENT_DIR=/path/to/lucent
```

The configure step refuses a missing or different Lucent revision. The option is
off by default, so ordinary LÖVE builds do not acquire the control channel.
