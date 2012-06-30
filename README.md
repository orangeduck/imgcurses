imgcurses
=========

About
-----

imgcurses is a smart console-based image viewer.

![House](https://github.com/orangeduck/imgcurses/raw/master/gallery/house.png)

Do you do all your internet browsing using lynx?
Do you wish the world was only 8 colors?
Do you pity your n00b peers and their pathetic desktops?

imgcurses might just be for you...

Unlike alternatives imgcurses is fully interactive and employs smart techniques to get the most out of viewing. It doesn't just map pixel values to ascii characters.

Gallery
-------

![Me](https://github.com/orangeduck/imgcurses/raw/master/gallery/me.png)
![Sunflower](https://github.com/orangeduck/imgcurses/raw/master/gallery/sunflower.png)

Controls
--------

To zoom use []

To scroll use the arrow keys.

To change mode press 'm' and to quit press 'q'.


Modes
-----

All modes are viewed best by scaling up the image as much as possible and then looking from afar and blurring your eyes. Used in combination it might become possible to recognize images but the limited color pallet makes almost any attempt impossible. If anyone has any bright ideas for new algorithms or techniques I'd more than welcome them.

* Solid Mode

  Simply evaluates the subsection of the image and uses the closest console color. The fastest mode.
  
* Value Mode
  
  A Black & White mode similar to other programs. Looks at the color and darkness of the pixel and chooses an appropriate ascii character. Good for overall contrast.
  
* Color Mode
  
  Blends between the two nearest colors using an appropriate ascii character. Best for trying to get a grasp of the color pallet, limited as it is.
  
* Detail Mode

  Looks at the sub-image detail and tries to match it to appropriate an ascii character. Best for seeing shapes and smaller details of the image.
  
Support
-------

Currently imgcurses only loads .tga files saved without compression.

