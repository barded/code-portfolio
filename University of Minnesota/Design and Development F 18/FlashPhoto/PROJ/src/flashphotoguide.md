Getting Started with FlashPhoto {#flashphotoguide}
===========
[TOC]

FlashPhoto is a minimal but powerful art application. With 7 tools and 8 filters, you will be able to create beautiful pieces from scratch, as well as import, edit, and save images.

This tutorial will introduce the tools and filters in FlashPhoto.

##Startup Screen
\image html fp-start.png

The startup screen is a white canvas with the toolbar on the left. The toolbar has four main sections. File will let you open and save images. Edit lets you undo and redo brush strokes and filters. Tools lets you quickly switch between tools, change color, and change the brush size. Finally, Filters will let you adjust settings for each, and apply them to the entire canvas.

##Color Picker
\image html fp-color.png

FlashPhoto’s compact color picker is great for quick access to adjust color. *Click* the colored bar to bring up the menu. To adjust the hue (red, green, blue), you can *click* anywhere in the outer circle, or *click and drag* for continuous and finer control.

To adjust the brightness and saturation, *click* or *click and drag* inside the triangle.

Finally, to set the color *click* the Pick button. If you don’t want to change the color *click* the Reset button.

##Tools
Below are sample strokes for each tool. You can either *click* to apply a dot to the canvas, or *click and drag* to apply a continuous stroke.

###Pen
\image html fp-pen.png

This tool is the most basic. It is a fully opaque (meaning nothing under a stroke will show through) and circular brush with hard edges. This tool is good for outlines, filling in color, and handwriting.

\image html fp-penbig.png Pen Tool with larger tool size.

You can adjust the size of any tool by *clicking* anywhere along the scroll bar, or *click and drag* for finer adjustment. **Note:** This applies to all scroll bars.

###Calligraphy Pen
\image html fp-cpen.png

Like the Pen Tool, the Calligraphy Pen is opaque with hard edges. However, as the above image shows, brush thickness depends on which direction you are painting. This is because the brush shape is an oval, rotated a little counter clockwise. Therefore, the line will be thinnest when moving from top left to bottom right, and thickest when moving from top right to bottom left. As the name suggests, this tool is good for calligraphy work, but can also make outlines look more dynamic.

###Chalk
\image html fp-chalk.png

This brush uses randomized transparent areas within the brush radius to more accurately resemble real chalk. The more you overlay strokes, the more it will look like the Pen Tool. This is because each stroke will cover more and more of the transparent areas. Chalk Tool is good for adding texture to your piece of art.

###Highlighter
\image html fp-highlighter.png

The green and purple strokes are the Pen Tool, and yellow is the Highlighter Tool. This tool is ovaluar and semi transparent (see through). How transparent it appears will depend on the color behind it, like a real highlighter. As well as highlighting, this tool is good for hard-edged shading.

###Spray Can
\image html fp-spray.png

The Spray Can is different than any other tool because it keeps applying the brush when you *click and hold* without moving the mouse. Fast strokes will result in high transparency and soft edges. Slow strokes will be more opaque with harder edges. This tool is good for airbrush effects.

###Blur
\image html fp-blur.png

Blur doesn’t apply color, but takes all the colors under the brush and mixes them by a small amount to make colors appear blurred. The top half of the image shows a small tool size for blur, while the bottom half has a much bigger size. A larger size will cover more canvas area and thus will have a bigger effect. Blur can be used with the Chalk Tool to create a colored pencil smudge effect, or with transparent tools to achieve more realistic shading.

###Eraser
\image html fp-eraser.png

As the name suggests, this tool will erase all colors, and leaves the background color. The edges are hard. At this time it is not possible to change the opacity, but we hope to add this feature in the future. This tool is good for, well...erasing!

##Filters

In this section we will briefly discuss what each filter and adjustments do. Many filters have an “Amount” adjustment. The larger the amount, the higher the effect on the image the filter has. Filters will apply effects to the entire canvas. If you don’t like how a filter changed the image, *click* the Undo button.

To show the effects of each filter, we’ve used FlashPhoto’s Open button to import an image of a flower. **Note:** Only .png files are accepted for opening and saving.

###Blur

\image html fp-blurfm.png

The Blur Filter only has one adjustment, the amount. The Blur Filter has the same effect as the Blur Tool, but applies to the whole image. After making adjustments, *click* Apply.

\image html fp-blurf.png Result of Blur, amount: 5.

###Motion Blur
\image html fp-mbm.png

This filter is similar to Blur, but with a direction adjustment. *Click* the bar with “North/South” text to see direction options. Then *click* the direction you want. Imagine taking a picture of someone jumping. They will probably be a little blurry in the vertical direction. This is the effect “North/South” will have on the image. Likewise, “East/West” will blur horizontally. After making adjustments, *click* Apply.

\image html fp-mb.png Result of Motion Blur, amount: 5, direction: North/South.

###Sharpen

\image html fp-sharpenm.png

Sharpen only has one adjustment, amount. This filter looks for abrupt changes in color, which usually is an edge. When an abrupt change is found, the filter will adjust the colors more so their brightness is farther apart. The image will look like it has crisper edges. Depending on the image, a smaller amount of adjustment will have greater effect. After making adjustments, *click* Apply.

\image html fp-sharpen.png Result of Sharpen, amount: 5.


###Edge Detect

\image html fp-edgem.png

Edge Detect has no adjustments, and works a lot like Sharpen. However, if nearby colors are similar, the filter will turn the area black. This way, only edges stand out and are colored. The result looks a bit like neon edges. *Click* Apply.

\image html fp-edge.png Result of Edge Detect.

###Threshold

\image html fp-thresholdm.png

Threshold has one adjustment, amount, but unlike other amount options, this goes from 0 to 1. Each pixel of color has a red, green, and blue component. Different amounts of each produce varying colors. The threshold filter takes an average of the components that is always between 0 and 1. If the average is less than or equal to the threshold amount, the color becomes black. If the average is greater, the color becomes white. A lower threshold amount will result in more black, and higher will result in more white. After making adjustments, *click* Apply.

\image html fp-threshold.png ‘Result of Threshold, amount: 0.5.

###Saturate

\image html fp-satm.png

Saturate also has only one adjustment, amount. When an image has a high saturation amount, such as 2 or more, the colors will look boosted and more intense. An amount between 0 and 1 will take color out of an image, and an amount set to 0 will make an image grayscale. Saturate changes red, green, and blue by the same amount. After making adjustments, *click* Apply.

\image html fp-sat.png Result of Saturate, amount: 2.88.


\image html fp-sat0.png Result of Saturate, amount: 0.0.


###Adjust R,G,B (Channels)

\image html fp-channelsm.png

Adjust R,G,B, also known as Channels, is similar to Saturate. However, now each color component can be adjusted separately. This allows you to boost and dim the intensity of red, green, and blue separately. This gives you finer image editing control. This is a common filter professional image editors use, and can create impressive images with some practice. After making adjustments, *click* Apply.

\image html fp-channels.png Result of Adjust R,G,B, red: 0.0, green: 1.89, blue: 2.42.

###Quantize

\image html fp-quanm.png

The last FlashPhoto filter is Quantize, with a Number of Bins adjustment. Each color component (red, green, blue) can be any decimal between 0 and 1. There are a lot of options! The Number of Bins adjustment lets you decide how many options there are between 0 and 1.

For example, if Number of Bins is 2, then each color component will be rounded to 0 or 1.
If Number of Bins is 3, then each color component will be set to 0, 0.5, or 1, whichever is closest to the actual value.

After making adjustments, *click* Apply.

\image html fp-quan.png Result of Quantize, Number of Bins: 2.

##Conclusion

We hope you have enjoyed and found this tutorial helpful. FlashPhoto is a simple, but powerful, application with ease in mind. By using our tools and filters, you can open and modify images, or start with a blank canvas and create something original. The possibilities are endless!

Learn about our other art program, [MIA](miaguide.html).
