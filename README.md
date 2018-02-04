# PROF2FITS
## Program which converts PROFS to FITS



### Installation
Makes use of `cfitsio` which are bundled with `prf2fits`. 

It also makes use of `boost/spirit/qi` to do parsing. 

The only way to use that functionality of boost right now is to run 

`sudo apt-get install libboost-all-dev` . 

*We want this program to be stand alone, and have no dependencies for which `apt-get` has to be used.*

`Makefile` has been crafted which installs `prf2fits`, `cfgen` and sets up the `cfitsio` dependencies on its own. 

You may want to add the path of this directory to your `PATH` environmental variable so that you can use the binaries generated here.

Following is a simple illustration of installing these programs:

`sudo apt-get update && sudo apt-get install libboost-all-dev`

`git clone https://github.com/shiningsurya/prf2fits.git`

`make install`


### Note

`CFITSIO` is a library built for FITS file IO in C, hence the name. Consequently, there are a lot warnings such as 

`warning ; ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]`

Hence, even though it is against the standard practises, we suppress the warnings by passing `-Wwrite-strings`.
Check out the criticism in [StackOverflow](https://stackoverflow.com/questions/8356223/assign-a-string-literal-to-a-char).


### Documentation
Documentation for this code is being written. 
It will be put here once it's ready. 


### Bugs/Questions/Comments/Suggestions ?
Suryarao Bethapudi 

Email : ep14btech11008 [at] iith [dot] ac [dot] in

Or, you can just make an issue in the Github repo.


