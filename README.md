# UNICEF-Project
## Overview
This is an application made for UNICEF in the arm intern competition 2018! The application is designed to allow a user to input
files describing a set of schools and hospitals (in a set format) from anywhere over the world, and then quickly be able to search
and extract important data that will help plan for development to make children safer and get the education they deserve.

## Our take
Although we had access to shapefiles, we thought that displaying a map was not that important, what really matters is ease of use and
ability to get and visualise the data that you are looking for. For this reason, we have decided that our innovation is to display the
data as a graph, much like the London tube map. The map will be interactive and can represent more than just paths between places. We propose
3 different modes
* Straight line distance mode
* Road distance mode
* Travel time distance mode

Sadly, due to the small size of our team, the software is only tested on Linux. We assume that building for windows
could be problematic, so get in contact if you need help.

## How to install
1. Clone this project (the master branch _should_ always work) and download QT from [here](https://www.qt.io/download)
2. When installing QT, be sure to use the newest version!
3. Open the project, a message about versions may come up, ignore it.
4. Hit run!

If you want to use it on a platform without QT Creator, follow [these](http://doc.qt.io/qt-5/deployment.html) instructions on deploying a QT app.
Although untested, the app should run on Android and iOS without modification.

## How to use
### General Use
When the software is running, you will see a bar at the top with three buttons. Use the first two to select the schools JSON file and Health Facilities geojson file.
After this, click import. The application will take a second, but soon, the results pane on the left side of the screen will be filled up with all the schools and health facilites.
On the right side of the screen you will see a circle. This is the simplified map. Clicking on a school or hospital will place that location in the center of the circle and
plot all health facilites (if you selected a school) or schools (if you selected a health facility). The JSON files are quite full in places, so to get a good feel of the application running best, search for 'Pozo
Frontera' to see a less cluttered display. You can change the search radius and change between kilometers and miles.

### Bing Maps Intergration
You can intergrate the application with bing maps for a better experiance. By default, clicking on a place
(not the center one), will open, in bing maps, the place you clicked on, and clicking on a blue line will bring
up directions between the selected place and the place that the blue line terminates at. But we can go further.
At the bottom, you will see "line length = " and some options. The point of our application is to get the
information you need far quicker than on a normal map, selecting Road distance will change it so that the
lengths of each line represent the distance by road from the selected place to the terminal place. Travel time
will make it update to say how many muinutes. In order to use this feature though, you need a bing API key which
can be got [here](https://www.bingmapsportal.com/).

Any more questions, please email me at james.hobson@arm.com

## A bit of Technical Info
Becuase we are prioritising speed, we have built the program to be as fast at rendering schools and
hospitals as possible, for this reason, we index the data twice upon importing. The first time, each
place is loaded into a custom data structure that is very efficient for searching by name. The next,
it is put into a tree structure that means that the work the computer has to do to find nearby places
is much decreased. We decided to use QML to build the render. This is becuase we could really easily
make the simple UI that we wanted and connect components together so that we could ripple changes with
far less going wrong. We chose QT for another reason too, it works on so many platforms! This app
could be put on an ipad with very little if not no extra modifications. C++ was the obvious choice as
speed matters to us!

## Test Coverage
As there has only been 2-3 active member on this project, we have only had the chance
to test on Linux. Please share your findings and fixes on other platforms! We imagine
MS Windows could be problematic.

## Progress
- [x] Parse JSON files
- [x] Search Schools and health facilities by name
- [x] Search UI
- [x] Search schools and facilities by location
- [x] General UI
- [x] QML renderer of graph
- [x] ~~Google~~ Bing map integration

![LOGO](./logo.png)
