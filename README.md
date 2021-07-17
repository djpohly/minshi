# [minshi]

minshi is a simple program, in the spirit of [srandrd](https://github.com/jceb/srandrd),
which listens for monitor connect/disconnect events and prints the event info to
standard output.  For a more full-fledged output layout profile manager, see
[kanshi], of which minshi is a minimal, stripped-down version (hence the name).

When an output is connected, minshi will print a single line,

    connect <TAB> <output name>

and when an output is disconnected, minshi will print

    disconnect <TAB> <output name>

minshi can be used on any Wayland compositor which implements the wlr-output-management protocol.

## Building

Dependencies:

* wayland-client
* scdoc (optional, for man pages)

```sh
meson build
ninja -C build
```

## Usage

```sh
minshi | handler-program
```

where `handler-program` is any executable program or script that will handle
monitor events.  If you would like multiple things to happen on connect or
disconnect, you can bundle that functionality into a single handler, or
you can also run multiple instances of minshi.

## License

MIT

[kanshi]: https://wayland.emersion.fr/kanshi/
[srandrd]: https://github.com/jceb/srandrd/
