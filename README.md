imgcurses
=========

About
-----

imgcurses is a smart console based image viewer.

![House](https://github.com/orangeduck/imgcurses/raw/master/gallery/house.png)

* Do you browse the internet using lynx?
* Do you wish the world was only 8 colors?
* Do you pity your peers with their pathetic desktops?

imgcurses might just be for you...

The limited color pallet introduces some pretty bad limitations, but unlike alternatives imgcurses is fully interactive and employs smart techniques to get the most out of viewing - it doesn't just map pixel values to ascii characters. Images are best viewed scaled up, from a distance, and with a blurred focus. The combination of modes highlights different image features and makes an overall understanding of the image much more plausible.

Gallery
-------

![Me](https://github.com/orangeduck/imgcurses/raw/master/gallery/me.png)
![Sunflower](https://github.com/orangeduck/imgcurses/raw/master/gallery/sunflower.png)
![Car](https://github.com/orangeduck/imgcurses/raw/master/gallery/car.png)

Controls
--------

* To zoom use '[]'
* To scroll use the arrow keys.
* To change mode press 'm'
* To quit press 'q'.


Modes
-----

If anyone has any bright ideas for new algorithms or techniques I'd more than welcome trying to implement them.

* Solid Mode - Simply evaluates the subsection of the image and uses the closest console color. The fastest mode.
* Value Mode - A Black & White mode similar to other programs. Looks at the color and darkness of the pixel and chooses an appropriate ascii character. Good for overall contrast.
* Color Mode - Blends between the two nearest colors using an appropriate ascii character. Best for trying to get a grasp of the color pallet, limited as it is.
* Detail Mode - Looks at the sub-image detail and tries to match it to appropriate an ascii character. Best for seeing shapes and smaller details of the image but can be very expensive.

File Formats
------------

Currently imgcurses only loads .tga files saved without compression.

