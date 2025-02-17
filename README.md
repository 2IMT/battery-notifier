# battery-notifier <a id="battery-notifier"></a>

CLI tool that notifies you when your battery is low.

## Table of contents <a id="table-of-contents"></a>

- [battery-notifier](#battery-notifier)
- [Table of contents](#table-of-contents)
- [Usage](#usage)
- [Configuration](#configuration)
- [Build](#build)
  - [Dependencies](#dependencies)
  - [Building](#building)
  - [Installation](#installation)
  - [Installation (Arch-based distros)](#installation-arch-based)

## Usage <a id="usage"></a>

Just start it and let it run in background:

```bash
battery-notifier
```

## Configuration <a id="configuration"></a>

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

## Build <a id="build"></a>

Note: Arch-based distro users may skip to [Installation (Arch-based distros)](#installation-arch-based) section

### Dependencies <a id="dependencies"></a>

- C compiler
- Make

### Building <a id="building"></a>

```bash
make
```

### Installation <a id="installation"></a>

Note: this may require root privileges.

```bash
make install
```

This will install the binary to `/usr/local/bin` if the `INSTALL_PREFIX` environment variable is not set. Set the `INSTALL_PREFIX` environment variable to change the installation directory.

### Installation (Arch-based distros) <a id="installation-arch-based"></a>

Use [this](https://aur.archlinux.org/packages/battery-notifier) AUR package
