# b2dwm ("Box2D Window Manager")

Ever thought floating window managers were too unrealistic? Well, me neither, but here's the solution!

This is based off of sowm, I just patched box2D in!

[Video here](https://www.reddit.com/r/unixporn/comments/ft8iow/b2dwm_i_think_floating_window_managers_arent_very/)

## Default Keybindings

**Window Management**

| combo                      | action                 |
| -------------------------- | -----------------------|
| `Mouse`                    | focus under cursor     |
| `MOD4` + `Left Mouse`      | move window            |
| `MOD4` + `Right Mouse`     | resize window          |
| `MOD4` + `f`               | maximize toggle        |
| `MOD4` + `c`               | center window          |
| `MOD4` + `q`               | kill window            |
| `MOD4` + `1-9`             | desktop swap           |
| `MOD4` + `Shift` +`1-9`    | send window to desktop |
| `MOD1` + `TAB` (*alt-tab*) | focus cycle            |
| `MOD4` + `g`               | toggle gravity         |

**Programs**

| combo                    | action           | program        |
| ------------------------ | ---------------- | -------------- |
| `MOD4` + `Return`        | terminal         | `st`           |
| `MOD4` + `d`             | dmenu            | `dmenu_run`    |
| `MOD4` + `p`             | scrot            | `scr`          |
| `MOD4` + `w`             | wallpaper cycler | `bud`          |
| `XF86_AudioLowerVolume`  | volume down      | `amixer`       |
| `XF86_AudioRaiseVolume`  | volume up        | `amixer`       |
| `XF86_AudioMute`         | volume toggle    | `amixer`       |
| `XF86_MonBrightnessUp`   | brightness up    | `bri`          |
| `XF86_MonBrightnessDown` | brightness down  | `bri`          |


## Dependencies

- `xlib` (*usually `libX11`*).
- `box2d`


## Installation

1) Copy `config.def.h` to `config.h` and modify it to suit your needs.
2) Run `make` to build `b2dwm`.
3) Copy it to your path or run `make install`.
    - `DESTDIR` and `PREFIX` are supported.

## Thanks

- sowm
- 2bwm
- SmallWM
- berry
- catwm
- dminiwm
- dwm
- monsterwm
- openbox
- possumwm
- swm
- tinywm
