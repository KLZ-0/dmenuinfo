# Status text generator for swaybar

Outputs a formatted line to stdout which can be used as input for a status bar

## Example
![Screenshot](img/screenshot.png?raw=true "Example")

## Compiling and installing

Clone the git repo:
```
https://github.com/KLZ-0/dmenuinfo.git
```

Run the installation script to compile and install a `Release` build: 
```
./install.sh
```

The installation directory is by default `~/.local/bin` which should be included in your `$PATH`

## Modifying the sway config

Modify the `status_command` in `.config/sway/config`:
```
bar {
    status_command while ~/.local/bin/systatus; do sleep 1; done
}
```
