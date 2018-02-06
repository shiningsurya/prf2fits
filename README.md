# PROF2FITS
## Program which converts PROFS to FITS



### Installation
Makes use of `cfitsio` which is bundled with `prf2fits`. 

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

Hence, even though it is against the standard practises, we suppress the warnings by passing `-w`.
Check out the criticism in [StackOverflow](https://stackoverflow.com/questions/8356223/assign-a-string-literal-to-a-char).

We also have to pass `-fpermissive` since `const void *` is re-interpreted as `void *` in the CFITSIO routines. This means we've to be extra careful in not changing strings in any case. 

### Documentation
You can make the documentation (if you can't find it already), by running `make doc`. 
It will create a `manual.pdf`. 

### Bugs/Questions/Comments/Suggestions ?
Suryarao Bethapudi 

Email : ep14btech11008 [at] iith [dot] ac [dot] in

Or, you can just make an issue in the Github repo.


