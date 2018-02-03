# PROF2FITS
## Sample program which converts PROFS to FITS



### Installation
Makes use of `cfitsio` which are bundled with `prf2fits`. 

It also makes use of `boost/spirit/qi` to do parsing. 

The only way to use that functionality of boost right now is to run 

`sudo apt-get install libboost-all-dev` . 

*We want this program to stand alone*

`Makefile` has been crafted which installs `prf2fits`, `cfgen` and sets up the `cfitsio` dependencies on its own. 

You may want to add the path of this directory to your `PATH` environmental variable so that you can use the binaries generated here.

Following is a simple illustration of installing these programs:

`sudo apt-get update && sudo apt-get install libboost-all-dev`

`git clone https://github.com/shiningsurya/prf2fits.git`

`make install`


### Documentation
Documentation for this code is being written. 
It will be put here once it's ready. 


### Bugs/Questions/Comments/Suggestions ?
Suryarao Bethapudi 

Email : ep14btech11008 [at] iith [dot] ac [dot] in

Or, you can just make an issue in the Github repo.


