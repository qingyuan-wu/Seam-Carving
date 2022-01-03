# Seam-Carving

Seam carving is an image-resizing technique that removes one vertical "seam" of pixels at a time.

[Video Demo](https://www.youtube.com/watch?v=9WJvDcoNFzA)

* See `seamcarving.c` for functions to compute the removal of the seam with minimum energy. A *dual energy gradient function* was used to compute energies. In general, two adjacent pixels that are similar in their RGB value have lower energies - these pixels will be removed first.
* `c_img.c` writes RGB values to a .bin file to represent the image.
* `bin_to_png.py` is used to first convert an image to binary. After the images is seam carved, it can be converted back to a .png file with `bin_to_png.py`. 
