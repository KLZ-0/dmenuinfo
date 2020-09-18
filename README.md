# Status text generator for swaybar

Outputs a formatted line to stdout which can be used as input for a status bar

## Example
![Screenshot](img/screenshot.png?raw=true "Example")

## Compiling and installing 

Note: nmcli is required as a runtime dependency, in case you don't use networkmanager you may want to comment `PRINT_NETWORKNAME`

Clone the git repo:
```
git clone https://github.com/KLZ-0/systatus.git && cd systatus
```

Compile a `Release` build: 
```
make
```

Install to `~/.local/bin`:
```
make install
```

The installation directory is by default `~/.local/bin` which should be included in your `$PATH`

## Modifying the sway config

Modify the `status_command` in `.config/sway/config`:
```
bar {
    status_command while ~/.local/bin/systatus; do :; done
}
```

Or if `~/.local/bin` is in your `$PATH`:
```
bar {
    status_command while systatus; do :; done
}
```

## Options

Usage: `systatus [-hvubsknd]`

```
-h      this help message
-v      systatus version
-u      uptime
-b      battery percentage
-s      battery status
-k      kernel version
-n      connected networks
-d      date in ctime format
```
