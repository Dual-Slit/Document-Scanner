# Document-Scanner
This is Sample Document Scanner using of OpenCV C++

It is an OpenCV C++ project

Project Name : Document Sanner
Written By : Ashutosh

This code need Microsoft Visual Stdio for good results

some settings need to be done in Microsoft Visual Stdio 
1.Nevigate Project Properties 
a.==> VC++ Directories --> Include Directories --> add your <opencv\buil\include> path 
b.==> VC++ Directories --> Library Directories --> add your <opencv\buil\x64\vc16\lib> path
c.==> linker --> input --> additional dependencies -->

While running/compilation of code ==> Since this code takes input from your present Directory, if single webcam is attached then no need to change otherwise chnage web cam ID (line no 13)

This program works fine for Green and Purple Colour, if you need more colour add/remove then in myColors vector(line no 20) add its HSV minimum and maximum values and add corresponding color in myColorValues vector (line no 24)
