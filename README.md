# SketchyVim
This small project turns all accessible input fields on macOS into full vim
buffers. It should behave and feel like native vim, because, under the hood
I simply sync the textfield to a real vim buffer.

You can use all modes (even commandline etc.) and all commands included in vim.

It is also possible to load a custom `svimrc` file, which can contain
custom vim configurations, e.g. remappings (see the examples folder).

Additionally, you can edit the `blacklist` file in the `~/.config/svim/` folder
to manually exclude applications from being handled by svim.

Every time the vim mode changes, or a commandline update is issued, the script
`svim.sh` in the folder `~/.config/svim/` is executed where you can handle 
how you want to process this information. I have a small popup in my SketchyBar
which shows me the commandline output on demand for example.

## Installation
You can install this using brew from my tap:
```bash
brew tap FelixKratz/formulae
brew install svim
```
and then you can start the brew service using:
```
brew services start svim
```
where you will be asked to grant accessibility permissions.

## Issues
Please tell me if you encounter issues.

Known Issues:
-------------
* Multikey remappings are not recognized (e.g. jk for esc)
* The cursor sync is not working correctly in the case where something is selected with the mouse in insert mode and then deleted.
