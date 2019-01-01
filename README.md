# PROF2FITS
## Program which converts PROFS to FITS


### Installation
Makes use of `cfitsio` which is bundled with `prf2fits`. 

`git clone https://github.com/shiningsurya/prf2fits.git`

*install CFITSIO*
- `sudo apt-get install libcfitsio3-dev`

`make all`

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

You can make an issue in this repo.


