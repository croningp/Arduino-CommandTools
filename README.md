# CommandTools

A variety of library working together to ease the setup and control of Arduino devices. Tools are generic and can be approached and used at different levels.

The [commanduino](https://github.com/croningp/commanduino) python library complement these tools by allowing automatic discovery and instantiation of devices on multiple Arduino boards using a simple config file.


## Install

You should have arduino IDE, python, and git installed.

### Linux or Mac OS X

Use symbolic links.

Open terminal:

```shell
mkdir yourfavoritefolder
cd yourfavoritefolder
git clone https://github.com/croningp/Arduino-CommandTools.git
cd Arduino-CommandTools
python symlinks.py --install #Makes symbolic links to libraries in /home/<yourusername>/sketchbook
```

Test your installation by compiling the demo of CommandHandler.

### Windows

Clone where you want and copy/paste libraries you want to use in the appropriate library folder.

Test your installation by compiling a demo.

Any symbolic link implementation for Windows is welcomed.


## Credit

The ```symlinks.py``` script comes from https://github.com/janelia-arduino/arduino-libraries

## Authors

[Jonathan Grizou](http://jgrizou.com/) while working in the [Cronin Group](http://www.chem.gla.ac.uk/cronin/). 

Contributions by [Graham Keenan](https://github.com/ShinRa26) and [Jean-Patrick Francoia](https://github.com/JPFrancoia).

## License

[![GPL V3](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl.html)
