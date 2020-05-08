Getting Started with MIA {#miaguide}
===========


MIA stands for Medical Image Analysis, and is crafted specifically for speed and efficiency for medical professionals. It’s also handy for anyone needing a quick way to annotate an image or apply basic filters. MIA has a command line mode for streamlining editing a large number of images.

This tutorial will introduce how to open an image, annotate it, and save the image.


## Startup Screen ###


\image html mia-start.png

The startup screen is a white canvas with the toolbar on the left. The toolbar has four main sections. File will let you open and save images. Edit lets you undo and redo brush strokes and filters. Tools lets you quickly switch between tools, change color, and change the brush size. Finally, Filters will let you adjust settings for each, and apply them to the entire canvas. To learn more about tools and filters, visit [Getting Started with FlashPhoto](flashphotoguide.html).

##Opening an Image

MIA makes opening an image in the application easy. Near the top of the toolbar, *click* Open. This will bring up a file browser. Navigate to your desired image and *click* OK. The application window will resize to fit the dimensions of the image. **Note:** Only .png files can be opened.

\image html mia-load.png A x-ray of a broken foot.

##Mark Tool

MIA’s Mark Tool is great for quickly marking a point of interest. This tool’s shape is a dot in the center with an outer circle, like a target or bullseye. In our example, we use this tool to mark the broken bones.  

\image html-mark.png

##Color Picker
\image html mia-color.png

MIA’s compact Color Picker is great for quick access to adjust color. *Click* the colored bar to bring up the menu. To adjust the hue (red, green, blue), you can *click* anywhere in the outer circle, or *click and drag* for continuous and finer control.

To adjust the brightness and saturation, *click* or *click and drag* inside the triangle.

Finally, to set the color *click* the Pick button. If you don’t want to change the color *click* the Reset button.


##Pen Tool

\image html mia-pen.png

The Pen Tool is pretty basic, but is great for quickly writing on an image, circling points of interest, drawing arrows, etc. It is a fully opaque (meaning nothing under a stroke will show through) and circular brush with hard edges. In our example, we used this tool to label each break.

##Saving an Image

MIA makes saving an image in the application just as easy as opening one. Near the top of the toolbar, *click* Save. This will bring up a file browser. Navigate to your desired folder to store the image, type the name you’d like the image to have, and *click* OK. **Note:** Only .png files can be saved.

\image html mia-save.png The annotated image is saved as foot-marked.png

##Command Line Mode
\image html foot.png

With Command Line Mode, you can open, edit, and save an image with your computer’s terminal application. Open a terminal, and navigate to the folder containing MIA application.

Here is generally how to specify what you’d like MIA to do: `./mia infile.png [image processing commands ...] outfile.png`

For example, if we wanted to sharpen foot.png by an amount of 6, we type:
`./mia foot.png -sharpen 6 foot-sharpen6.png`

\image html foot-sharpen6.png foot-sharpen6.png
For more information on Command Line Mode, see [command_line_processor.h](classimage__tools_1_1CommandLineProcessor.html).

##Conclusion
We hope you have enjoyed and found this tutorial helpful. MIA is a small application built with the medical field in mind. MIA’s small selection of tools and filters is ideal for quickly annotating images. The Command Line Mode is great for editing a large number of images using scripts. Even if you’re not a medical professional, MIA can help you complete your image editing needs!

Learn about our other art program, [FlashPhoto](flashphotoguide.html).
