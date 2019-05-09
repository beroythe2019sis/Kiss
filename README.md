# Kiss
trying to make a process journey that may be used to make a new path

all .gh files can be converted to text files using .ghx


Arduino

  Pins (working out which pins crossover with each othe)
  
  
  Connecting multiple arduinos (i2c)
    https://www.youtube.com/watch?v=RPHP4fAisz8
    https://www.youtube.com/watch?v=6IAkYpmA1DQ
    
  Connecting to the wifi module (ESP8266)
        build a sensing component that is wireless (IOT)
        esp8266 (board I bought from Jaycar Duinotech uses CH340/341 Serial to USB signal processing, which would not load correctly on my Lenova Yoga 530, windows 10 - so abandoned for this project
          ; CH341SER.INF
          ; Driver for CH340/CH341 (USB=>SERIAL chip) V3.5
      esp32 (another wifi enabled board, as a development board, the route to understanding the hardware will take too long for this projet
  
  
  DHT11 with interupt - looked on github for code that uses an interupt to bypass the delay of 1 hz
      
  
  Signal processing options 
    rs485, i2c, wifi, ICSP-USB (SERIAL)
    
    
    
  Amplification options
    http://www.ti.com/lit/ds/symlink/ina333.pdf
    
  Braccio
   https://www.youtube.com/watch?v=xwg6QIFX3yw
   https://www.youtube.com/watch?v=eKAhQHApFGQ
   https://www.youtube.com/watch?v=qXCYMTAFbzk
   https://www.youtube.com/watch?v=xiMoGeVya8o
   https://www.youtube.com/watch?v=GFa6zeSobao
  
Rhino
github page
http://rhino.github.io/

  Plugins used
    Grasshopper
      Primary grashopper Tutorials referenced

      Data
      https://www.youtube.com/watch?v=ZPM6Ppip6EY
      https://www.youtube.com/watch?v=SmNPxKTDcQI&t=2s
      https://www.youtube.com/watch?v=9TvVahUs3cQ
      https://www.youtube.com/watch?v=q_MFx9-WO90&t=360s
      https://www.youtube.com/watch?v=lH5dE9vHKwQ&t=201s

      Animation
      https://www.youtube.com/watch?v=qHtRqGRnDBM
      
      Evaulating surfaces
      https://www.youtube.com/watch?v=PQlseAzm1b4
      https://www.youtube.com/watch?v=RLQuKrW9-YI&t=16s
      https://www.youtube.com/watch?v=nayHHsyuP0Y&t=368s
      https://www.youtube.com/watch?v=AP5PxBn_5rc - this one may have most useful answers
      
      Pattern in Pattern
      https://www.youtube.com/watch?v=dWt8PfYZOh8
      
      Dead Ends
      loops - other ways to loop in grashopper than a loop command
      Automatic Baking - no need to bake a surface for the moment
      Meshing - real world forces, which I am less interested at representing
      Lady Bug - graphing tool, feel too much like a control
      Lunchbox - grasshopper plugin
      WeaverBrid - grasshopper plugin


    Potential analogies to explore
    code - decode - same topology, with different paths (course material)
    tunnel paths electrons - https://chem.libretexts.org/Courses/Grand_Rapids_Community_College/CHM_120_-_Survey_of_General_Chemistry/3%3A_Chemical_Formulas_and_Bonding/3.03%3A_Chemical_Bonds
    Theory of everything using patterns and quantam theory - https://www.youtube.com/watch?v=Rqu_uV-gIcU
    auxetics - firemans rope - https://www.youtube.com/watch?v=tJ7PdXLb4O8&t=163s

    Tring to create open source code sharing limitations from Rhino website
    https://discourse.mcneel.com/t/share-definitions-on-github/47965/8
      Look for a way to break out smaller parts to share as modules
    
    Firefly
    https://www.youtube.com/watch?v=5cZYRzsR5Uw&t=42s
    
    Ghpython
