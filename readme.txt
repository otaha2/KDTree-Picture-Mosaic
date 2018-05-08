This MP was about making a picture mosaic from a large group of individual photos. This was accomplished through the use of KD-Trees and the FindNearestNeighbor function in particular. Below are the steps to run the program and make your own photo mosaic. 

Note: 

1) All photos must be in PNG format at a recommended size of no more than 200KB otherwise the program will take a long time to process 

2) A folder of at least a couple hundred unique photos is great to get the best output image, a thousand photos will give you an even better output

3) Was developed on Linux system, do not know the outcome of running on different system


commands to run:

make

./mp5 background_image.png [tile_directory/] [number of tiles] [pixels per tile] [output_image.png]

Note:

//background_image.png is the source image you would like the mosaic to be of (an example image is provided as Nature.png)
//tile_directory is the folder in which all of your individual photos are (an example folder has been provided as mp5_pngs) 
//number of tiles: 150 or more works very well, anymore than 300 is a bit excessive
//pixel per tile: recommended less than 30 otherwise mosaic image will be very large
//output_image.png is the name you wish to call your outputed image. This is the final mosaic outputted by the program

For Example Running:

make

./mp5 Nature.png mp5_pngs 150 25 NatureMosaic.png

will produce the output you see as NatureMosaic.png from all of the png photos in the mp5_pngs folder

Try it out with your own photos and Enjoy!



*This was made as a class MP for CS225 at UIUC. Not all of the code shown was made by the students. In particular the student implemented the KDTree class and the mapTiles class and then used the PNG templatized class provided by the CS225 Course Staff*


