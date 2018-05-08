This MP was about making a picture mosaic from a large group of individual photos. This was accomplished through the use of KD-Trees and the FindNearestNeighbor function in particular. Below are the steps to run the program and make your own photo mosaic. 

Note: 

1) All photos must be in PNG format at a recommended size of no more than 200KB otherwise the program will take a long time to process 

2) Was developed on Linux system, do not know the outcome of running on different system


commands to run:

make

./mp5 background_image.png [tile_directory/] [number of tiles] [pixels per tile] [output_image.png]

//background_image.png is the source image you would like the mosaic to be of (an example image is provided as nature.png)
//tile_directory is the folder in which all of your individual photos are (an example folder has been provided as mp5_pngs) 
//number of tiles: 75 or more works well
//pixel per tile: recommended less than 30 otherwise mosaic image will be very large
//output_image.png is the name you wish to call your outputed image. This is the final mosaic outputted by the program

For Example:

make

./mp5 nature.png mp5_pngs 75 25 NatureMosaic.png
