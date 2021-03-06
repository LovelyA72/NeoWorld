Vocal synthesis engine for UTAU, world4utau. 
(Please take note that Ameya made the original, I am just updating it)

This program is a UTAU resampler created by combining Masanori Morise's 
updated developement in the speech synthesis program called "WORLD"
together with EFB-GT created by CustomMaid, as an attempt
to update the original world4utau

For this resampler to work as intended,
Please delete any files related to the older world4utau
The resampler will still work otherwise but you may encounter bugs or
glitches if you're unable to do so.

I take Ameya's stand on the creation of this resampler as well.
Of which I will quote:
"Since I am making it without understanding the mechanism of WORLD 
well, I may be damaging the original quality of WORLD."

Please forgive me if the resampler isn't as good as you expect it to be.

This version of world4utau rely partially on .frq files to create
.dio files which are its version of frequency files.
Please generate the .frq files beforehand to ensure world4utau works.

Flaglist
Existing world4utau flags
· g flag
　Gender flag.
　Values from -99 to -1 make the voice sound more feminine.
　Values from 1 to 99 make the voice sound more masculine.
· P flag (0~100 def85)
　Peak compressor.

I have added a few flags to help improve the quality of world4utau. 
Take note that the flags are case sensitive.
The following flags are additional flags that can be used:
· R flag
　Use this flag to regenerate .dio, .ctspec and .d4c files
　Advisable to use after editing .frq files.
　Works similar to TIPS R flag.
· B flag (0~100 def50)
　Breathiness flag ported from other resamplers.
　Will function similar to WARP's B flag for values below 50.
　Retains tn_fnds's B flag for values above 50.
· O flag (-100~100 def0)
　One to one port of tn_fnds's O flag
　Change the voice "brightness" Specifying + values suppresses 
　lower frequencies and amplifies higher ones.

Take note that the default mode of rendering samples in world4utau is
Stretching

However, I have implemented a looping algorithm which can be
activated using the e flag.

· e flag
　Changes the rendering method from stretching to looping.
　This is the opposite of tn_fnds.  
　By using the e flag, long notes may sound smoother.

Again, I am emphasising that the DEFAULT method of rendering is
STRETCHING, e flag changes it to looping.

Since I implemented the looping algorithm by myself, in relation
to the world4utau's code, it may not be as good as tn_fnds's or
efb-gt's loop.  But I digress.

※ This program is licensed under GPLv3 (General Public License v3).
Please see copying.txt for details of the license.
When modifying and distributing the source code, 
below are two points that needs be taken note of.
> For applications created using this program,
  it is necessary to distribute the source code as well.
> The application must also be GPL licensed.
※ Any licensee who adheres to the terms and conditions is given 
  permission to modify the work, as well as to copy and redistribute 
  the work or any derivative version.

Special thanks and credit goes to the following people:
· Ameya for creating world4utau,
· CustomMaid for creating EFB-GT/WARP.
· Zteer for creating tn_fnds and
· M.Morise for creating the WORLD software

If you face any problems, feel free to contact me.
https://twitter.com/UtauZany


