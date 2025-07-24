# Prospector ZMK Module

All the necessary stuff for [Prospector](https://github.com/carrefinho/prospector) to display things with ZMK. Currently functional albeit barebones.

## Features

- Highest active layer roller
- Peripheral battery bar
- Peripheral connection status
- Caps word indicator
- Modifier status indicator (shows active Ctrl, Shift, Alt, GUI keys)

## Installation

Your ZMK keyboard should be set up with a dongle as central.

Add this module to your `config/west.yml` with these new entries under `remotes` and `projects`:

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: carrefinho                            # <--- add this
      url-base: https://github.com/carrefinho     # <--- and this
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: prospector-zmk-module                 # <--- and these
      remote: carrefinho                          # <---
      revision: main                              # <---
  self:
    path: config
```

Then add the `prospector_adapter` shield to the dongle in your `build.yaml`:

```yaml
---
include:
  - board: seeeduino_xiao_ble
    shield: [YOUR KEYBOARD SHIELD]_dongle prospector_adapter
```

For more information on ZMK Modules and building locally, see [the ZMK docs page on modules.](https://zmk.dev/docs/features/modules)

## Usage

For split keyboards, since the peripheral battery widget uses the order in which peripherals were paired to arrange the sub-widgets, after flashing the dongle, pair the left side first and then the right side. For more than two peripherals, pair them in a left to right order.

The layer roller shows layers' `display-name` property whenever available, and will fall back to the layer index otherwise. To add a `display-name` property to a keymap layer:

```dts
keymap {
  compatible = "zmk,keymap";
  base {
    display-name = "Base";           # <--- add this
    bindings = <
      ...
    >;
  }
}
```

### Modifier Status Widget

The modifier status widget displays minimalist symbols for active modifier keys:

- **⌃** for Control (Ctrl)
- **⇧** for Shift  
- **⌥** for Alt/Option
- **⌘** for GUI/Windows/Cmd key

The widget appears in the top-right corner of the display and updates in real-time as you press and release modifier keys.

## Configuration Options

The following configuration options are available in your `config/*.conf` files:

| Option | Description | Default |
|--------|-------------|---------|
| `CONFIG_PROSPECTOR_LAYER_ROLLER_ALL_CAPS` | Convert layer names to all caps | n |
| `CONFIG_PROSPECTOR_ROTATE_DISPLAY_180` | Rotate the display 180 degrees | n |
| `CONFIG_PROSPECTOR_MODIFIERS_WIDGET` | Enable modifier status widget | y |
| `CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR` | Use ambient light sensor for auto brightness | y |
| `CONFIG_PROSPECTOR_FIXED_BRIGHTNESS` | Fixed display brightness (1-100) | 50 |
