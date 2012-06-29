imgcurses
=========

About
-----

imgcurses is an ncurses ascii image viewer.

For fans of lynx and those two are too 1337 to use a normal image viewer.


Controls
--------

To zoom and scroll use [] and the arrow keys. To change mode press 'm' and to quit press 'q'.


Modes
-----

All modes are viewed best by scaling up the image as much as possible and then looking from afar and blurring your eyes. Used in combination it might become possible to recognize images but the limited color pallet makes almost any attempt impossible.

* Solid Mode

  Simply evaluates the subsection of the image and uses the closest console color.
  The fastest mode.
  
* Value Mode
  
  Looks at the color and darkness of the pixel and chooses an appropriate ascii character.
  Similar to conventional ascii images. Good for overall contrast.
  
* Color Mode
  
  Blends between the two nearest colors using an appropriate ascii character.
  Best for trying to get a grasp of the color pallet, limited as it is.
  
* Detail Mode

  Looks at the sub-image detail and tries to match it to appropriate an ascii character.
  Best for seeing shapes and smaller details of the image.
