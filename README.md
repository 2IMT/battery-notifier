# battery-notifier

CLI tool that notifies you when your battery is low.

## Usage

Just start it and let it run in background:

```bash
battery-notifier
```

## Configuration

`battery-notifier` is configured using environment variables.

The following environment variables are used:

- `BN_BATTERY_PATH` - Path to the battery device directory
  - Default: `/sys/class/power_supply/BAT0`
- `BN_ADAPTER_PATH` - Path to the power adapter device directory
  - Default: `/sys/class/power_supply/AC0`
- `BN_THRESHOLD` - Battery level threshold
  - Default: `15`
- `BN_PERIOD_SECS` - Period of battery state polling in seconds
  - Default: `30`
- `BN_NOTIFY_COMMAND` - Command to execute when battery level is below the threshold
  - Default: `notify-send "The battery is low" "Please connect the charger"`

## Build

### Dependencies

- C compiler
- Make

### Building

```bash
make
```

### Installation

Note: this may require root privileges.

```bash
make install
```

This will install the binary to `/usr/local/bin` if the `INSTALL_PREFIX` environment variable is not set. Set the `INSTALL_PREFIX` environment variable to change the installation directory.
