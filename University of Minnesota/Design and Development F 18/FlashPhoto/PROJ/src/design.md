High-Level Design {#design}
===========
This page will introduce all the classes in the project and describe how they work together to create FlashPhoto and MIA, graphical user interface (GUI) art applications.

[MinGfx](https://ivlab.github.io/MinGfx/index.html), [NanoGUI](https://nanogui.readthedocs.io/en/latest/), and [OpenGL](https://learnopengl.com/Introduction) libraries are used in FlashPhotoApp and MiaApp classes to aid with creating an interactive graphics window. To learn more about these libraries, click on the links.

In the overview subsection, the entire UML is provided as a roadmap. The following subsections will break the UML into pieces with further explanation.



##Overview##

\image html uml.png Entire project UML.

This project consists of four major class groups: tools, filters, image editor, and the applications, MIA and FlashPhoto. There are several essential helper classes that are used by the major classes: ColorData, PixelBuffer, FloatMatrix, MaskFactory, and ImageToolsMath.  

Filters, tools, and the helper classes are compiled into a library. This way, multiple applications can be created without having to duplicate implementation of the core functionality, which makes the project easier to maintain and expand. A number of design patterns are used, and their role will be explained with the classes that rely on them.

##ColorData, PixelBuffer, FloatMatrix, MaskFactory, and Tools
\image html uml-tools.png

###ColorData
Pixels have four components that determine color: red, green, blue, and alpha. Each has a range [0, 1]. For red, green, and blue, 0 corresponds to none of that color, 1 is fully that color.  Alpha controls transparency; 0 is fully transparent, and 1 is fully opaque. Luminance is a weighted average of red, green, and blue, and corresponds to the intensity of a pixel. Clamp is used to ensure component values are between 0 and 1.

###PixelBuffer
PixelBuffer objects are used to create the canvas or image out floats, which can be interpreted as ColorData objects. The raw pixel information is stored as floats in a vector of size `width * height * 4`, where width and height are the size of the canvas or image in pixels. We multiply by 4 because each pixel has 4 components. Pixels are in row-major order. `SaveToFile` and `LoadFromFile` are used by FlashPhoto and MIA to import and export .png files.  

###FloatMatrix and MaskFactory  
The FloatMatrix class has a vector of floats in row-major order, `float_array_`. There are also functions to manipulate the elements in the vector, such as `Sum()`, which returns the sum of all the elements. These are used when working with convolution filters, which will be discussed later.

Tool brush shapes, called masks, are made out of FloatMatrix objects by MaskFactory. As the name suggests, MaskFactory uses the factory design pattern. We use the factory design pattern in order to create and return a mask without having to make a MaskFactory object or reveal how the mask is made. The tools have three different mask options (constant, oval, and linear falloff) plus extra adjustments to make their visual brush unique.  

###Tools
Individual tools inherit from the Tool base class, and have a paint color, mask, and buffer. Each tool must fill in their own implementation for `CreateMask` using MaskFactory. The other virtual methods are optionally implemented based on desired effect of the tool.

The `buffer_` member variable is set equal to the canvas buffer in the `StartStroke` method so `AddToStroke` can easily apply the tool’s mask to the canvas. When a user clicks on the canvas, the GUI (FlashPhoto, MIA) will call `StartStroke`, and as the user clicks and drags, `AddToStroke` will  be called repeatedly. When the user releases the left mouse button, `EndStroke` is called, which sets `buffer_ = NULL`. `StampMaskOntoBuffer` and `CombinePaintAndCanvasColor` are used to combine the mask and the canvas together.

The blur tool is different from the others because it uses a localized version of the blur filter, `blur_`. Instead of adding color to the canvas, blur will mix colors within the mask area together. Look in the filter section for more information on the blur filter.

##Filters
\image html uml-filters.png

There are two types of filters: simple and convolution. Each type has four individual filters, and all inherit from Filter base class. The simple filter only looks at and edits the pixel at (x, y), which means they calculate in place. `CalculateFilteredPixel` is purely virtual, and this is what makes each filter unique. Within each filter implementation of this method, ImageToolsMath class may be used to aid in calculations.

Unlike simple filters, convolution filters take into account the pixels around the one being edited. This is done by using a kernel and convolving it with the canvas buffer. Each convolution filter is in charge of creating its own unique kernel, which is done in the `SetupFilter` method.  The FloatMatrix utility methods are used to help create the kernels.  Each convolution filter has a `size_` variable, which is the width and height of the kernel. Users will specify a radius, and size is determined by `2 * radius + 1`.   

As mentioned in the UML diagram, the `ApplyToBuffer` uses a template design pattern because the steps to apply a filter are  basically the same for both types. Virtual methods are used to allow for more flexibility in these steps based on the  type of filter.

##ImageEditor  
\image html uml-imageeditor.png

The ImageEditor class is the heart of the project. It is responsible for how tools and filters interact with the canvas or image. The class has member variables for each tool and filter, which are used to call object-specific implementations of methods.

The `current_buffer_` variable is the canvas or image we edit with tools and filters. The `current_tool_` is set by `GetToolByName`, and is used by `StartStroke`,`AddToStroke` and `EndStroke`, which then call the current tool’s implementation of each. Each filter has an  `Apply` method in ImageEditor. For example, this is how `f_blur_` is used:

    ApplyBlurFilter(float radius) {
      if (current_buffer_) {
        SaveStateForPossibleUndo();
        f_blur_.set_size(radius);
        f_blur_.ApplyToBuffer(current_buffer_);
      }
    }

FlashPhoto and MIA have options to undo and redo brush strokes or filters. To accomplish this, before the next change is made to the canvas, `SaveStateForPossibleUndo` will add the buffer to the `saved_states_` vector. Undo and redo will push and pop from the vector as needed.

In the model-view-controller paradigm, we can think of this class as implementing the model for an ImageEditor that could be controlled by various controllers (e.g., interactive mouse and keyboard, command line) and viewed different ways (e.g., interactively with a GUI, with files written to disk) (Keefe). FlashPhoto and MIA use an instance of ImageEditor to access and use the various tools, filters, and helpful functions such as PixelBuffer’s load and save to file.



##MiaApp and FlashPhotoApp
\image html uml-fpmia.png

MiaApp and FlashPhotoApp have the same structure and logic, but implement different sets of tools and filters. FashPhotoApp is an application for artists, so all filters and most tools are options, whereas MIA is for medical professionals and only includes 2 tools and 4 filters.

Both classes rely on MinGfx, NanoGUI, and OpenGL to enable interactive graphics. Variables like `thresh_cutoff_` are used by these libraries to transfer information from the GUI to the ImageEditor object. Here is an example:

      th_slider->setCallback([this, th_text](float value) {
        std::stringstream innerStream;
        innerStream << std::fixed << std::setprecision(2) << value;
        th_text->setValue(innerStream.str());
        thresh_cutoff_ = value;
      });

      b = new nanogui::Button(thresh_win, "Apply");
      b->setFixedSize({150, 30});
      b->setCallback([this]
        { image_editor_.ApplyThresholdFilter(thresh_cutoff_); });

In this example, a NanoGUI slider is used to set `value`, which is then used to set `thresh_cutoff_’. Then, when the user clicks Apply, `image_editor_.ApplyThresholdFilter(thresh_cutoff_);` will be executed. All filters work in a similar manner.

FlashPhoto has an enumerated type for motion blur. The GUI uses “North/South” string identifiers for the buttons. With `MBlurDir` and a map function (`MBlurDir` to string), we are able to cast and pass the direction into `image_editor_`.

      new nanogui::Label(m_blur_params, "Direction:", "sans-bold");
      nanogui::ComboBox *mbr_cb = new nanogui::ComboBox(
          m_blur_params, {MotionBlurDirectionName(MBLUR_DIR_N_S),
                          MotionBlurDirectionName(MBLUR_DIR_E_W),
                          MotionBlurDirectionName(MBLUR_DIR_NE_SW),
                          MotionBlurDirectionName(MBLUR_DIR_NW_SE)});
      mbr_cb->setFixedSize({216, 20});
      mbr_cb->setSelectedIndex(static_cast<int>(mblur_dir_));
      mbr_cb->setCallback([this](float value) {
        (void)value;
        int intValue = static_cast<int>(value);
        mblur_dir_ = static_cast<MBlurDir>(intValue);
      });

      b = new nanogui::Button(m_blur_win, "Apply");
      b->setFixedSize({150, 30});
      b->setCallback([this] { image_editor_.ApplyMotionBlurFilter(mblur_radius_,
        static_cast<ConvolutionFilterMotionBlur::BlurDir>(mblur_dir_)); });

In the above code, `MotionBlurDirectionName` turns the enumerated type to string for the GUI buttons, which returns a float that we cast into `MBlurDir`. When the user hits Apply, we cast `mblur_dir_` to the enumerated type `BlurDir` and the `image_editor_` processes the command.

When the user clicks a GUI tool button, the `tool_name_` variable is set:

    b = new nanogui::Button(tools);
    b->setFlags(nanogui::Button::RadioButton);
    b->setFixedSize({150, 20});
    b->setCaption(ToolChalk::name());
    b->setCallback([this]() { tool_name_ = ToolChalk::name(); });

Then, when the user clicks or clicks and holds the left mouse button, the following code is run:

    void FlashPhotoApp::OnLeftMouseDown(const mingfx::Point2 &pos) {
      tool_x_ = pos[0];
      tool_y_ = pos[1];
      image_editor_.StartStroke(tool_name_, tool_color_, tool_radius_,
                                tool_x_, tool_y_);
      painting_ = true;
    }

The above code shows how the class variables are used to pass tool information like color from the FlashPhoto or MIA application to ImageEditor.

In summary, FlashPhotoApp and MiaApp classes are used to translate between user interface libraries like NanoGUI and ImageEditor.


##MIA Command Line Mode
\image html uml-commands.png

In addition to GUI, MIA also has a command line mode. This allows users to quickly load an image, apply filters, and save the result. Here is an example command:

    ./mia infile.png -blur 5 out.png

This command loads infile.png, applies the blur filter with radius 5, and saves the result as outfile.png. Only .png files are accepted, and `FileExists()` checks that the image exists before attempting to load it. There is no limit on how many filters can be applied using the command line.

MIA has a `main.cc` file that will launch the GUI if no command line arguments are given. Otherwise, `main` creates an instance of CommandLineProcessor and call `ProcessCommandLine`. This will parse the arguments. For example:


        if (!strcmp(argv[0], "-blur")) {
         if (arg_count < 2) {
            std::cout << "Too few arguments for -blur." << '\n';
            return 0;
          } else {
            float r;
            try {
              r = std::stof(argv[1]);
            }
            catch (const std::invalid_argument &ia) {
              std::cout << argv[1] << " isn't a valid -blur radius." << '\n';
              return 0;
            }

            if (r < 1) {
              std::cout << "-blur radius needs to be greater than 0." << '\n';
              return 0;
            }

            commands_.push_back(new BlurFilterCommand(image_editor, r));
            arg_count -= 2;
            argv += 2;
          }

This is the `-blur` parser. First, it checks that there’s an additional argument, since the command also needs a radius. Second, a `try catch` block is used to make sure the radius is a  number, and if it is, make sure that it is in the valid range.  If the entire command is acceptable, a `BlurFilterCommand` with the desired radius is added to the `commands_` vector of pointers. Before executing any command, we make sure all commands are valid, and the vector allows us to save commands.

If the parser finds an error, either a detailed message or usage directions via `PrintHelp()` will be printed.

If all commands are valid, `ProcessCommandLine` loops through the `commands_` vector and runs:

    ImageEditorCommand *cmd = commands_[i];
    (*cmd).Execute();

Each command has its own subclass that implements `Execute()`. For example, this is how BlurFilterCommand class implements it:

    void BlurFilterCommand::Execute() { image_editor_->ApplyBlurFilter(radius_); }

In this example, `image_editor_`  is part of the ImageEditorCommand base class, and `radius_` is a variable in the BlurFilterCommand subclass. Each subclass has the necessary variables to call its corresponding method in the ImageEditor class.

**Note:**  `StartStrokeCommand`, `AddToStrokeCommand`, `EndStrokeCommand`, `RedoCommand`, and `UndoCommand` are not parsed in `ProcessCommandLine`.

##Conclusion
Throughout this page, the key elements of FlashPhoto and MIA were explained. Individual tools and filters inherit variables and methods from their base classes. Virtual methods allow subclasses to have their own implementation, which makes tools and filters unique. ImageEditor class  is used by FlashPhotoApp and Mia app as a library to connect the GUI with the core functionality of tools and filters. We hope you have found this design tutorial helpful.
