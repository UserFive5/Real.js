A Really Fast, Stupid And Samll QuickJs Runtime 
# Build Using :
```
   git clone https://github.com/Userfive5/Real.js 
   cd Real.js 
   gcc main.c -o main -lquickjs -lm -ldl -lcurl -lSDL2
```
# Build QuickJs Using :
   git clone https://github.com/bellard/quickjs
   cd quickjs
   make
   - After Done execute ` make install ` to install the static library
   - to compile to shared library to make building Real.js faster:
     `` mkdir shared && cp *.o shared/ ``
      `` cd shared && gcc *.c -o libquickjs.so -shared ``
   - then install with :
         `` cp libquickjs.so /use/lib/ ``
# Utils Right Now:
 # Console :
  `` console.log ``
   `` console.assert (with support for functions) ``
    `` console.prompt ``
     `` console.print ``
      `` console.getpass ``
 # @real/http :
   `` only fetch with no Utils ``
 # @real/fs :
     right now just read
# @real/sys :
     getenv ,setenv, environ , clearenv, exit, popen (Read-Only )
# @real/sdl2 :
     Empty Right Now
# Info :
   ``Have No Files Import Right Now``
   `` Have require method to get module.exports from a file ``

# So What IS This Project For? :
   This Is Just A Simple Project That I Worked On
   when I wanted to learn embedding JavaScript in c with quickjs
   So this just got to me and build it And I Will Really Work on
   To become a project that worth it!
# Something Awesome :
See Something Awesome : https://github.com/saghul/txiki.js
