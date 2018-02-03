#define _THIRD_H 1
#include<iostream>
#include<map>
#include<utility> // utility for pair
#include<vector>
#include<fstream>
//#include<boost/algorithm/string.hpp>
#include<string>
#include "fitsio.h"
#include "ioer.hpp"

// class IOer;
class FITS;
class PROFIO;


// definition of header struct
struct Header {
		std::string observer, project, telescope;
		double ant_x,ant_y,ant_z;
		std::string fend, fd_pol,bend,befcfg;
		int ibeam, numchans, fd_had;
		float fd_sang, fd_xy;
		int bep, bedcc, obsfch;
		float bede,obscf,obsbw,dm;
		double tcycle;
		std::string obsm, dobs;
		std::string pnid, reffm, srcn;
		float equinox;
		float bmaj, bmin, bpa;
		std::string raj,decj;
		std::string start_coord1, start_coord2;
		std::string tmode;
		std::string stop_coord1, stop_coord2;
		double slen;
		float fareq;
		std::string fdmode,calmode;
		double calfreq;
		float caldcyl, calph;
		int calnph;
		long stt_imjd, stt_smjd;
		double stt_offs, stt_lst;
};

// definition of from header struct
struct fromHeader {
		double mjd, fract, period;
		long numpulses;
		float freq, dm;
		int numbins, pol, numchans;
		char tid;
};
